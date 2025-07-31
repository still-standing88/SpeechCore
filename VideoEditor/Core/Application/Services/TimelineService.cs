using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using VideoEditor.Core.Domain.Entities;
using VideoEditor.Core.Domain.Interfaces;
using VideoEditor.Core.Domain.ValueObjects;

namespace VideoEditor.Core.Application.Services
{
    /// <summary>
    /// Service for managing timeline operations
    /// </summary>
    public class TimelineService : ITimelineManager
    {
        private readonly List<TimelineTrack> _tracks = new();
        private TimePosition _currentPosition = TimePosition.Zero;

        public event EventHandler<TimelineChangedEventArgs>? TimelineChanged;

        public TimePosition CurrentPosition
        {
            get => _currentPosition;
            set
            {
                if (_currentPosition != value)
                {
                    _currentPosition = value;
                    OnTimelineChanged(new TimelineChangedEventArgs(
                        TimelineChangedEventArgs.ChangeType.PositionChanged));
                }
            }
        }

        public void AddTrack(TimelineTrack track)
        {
            if (track == null) throw new ArgumentNullException(nameof(track));
            
            _tracks.Add(track);
            _tracks.Sort((a, b) => a.LayerIndex.CompareTo(b.LayerIndex));
            
            OnTimelineChanged(new TimelineChangedEventArgs(
                TimelineChangedEventArgs.ChangeType.TrackAdded, track.Id));
        }

        public bool RemoveTrack(Guid trackId)
        {
            var track = _tracks.Find(t => t.Id == trackId);
            if (track != null && _tracks.Remove(track))
            {
                OnTimelineChanged(new TimelineChangedEventArgs(
                    TimelineChangedEventArgs.ChangeType.TrackRemoved, trackId));
                return true;
            }
            return false;
        }

        public IReadOnlyList<TimelineTrack> GetTracks() => _tracks.AsReadOnly();

        public IReadOnlyList<TimelineTrack> GetTracksByType(TrackType type)
        {
            return _tracks.FindAll(t => t.Type == type).AsReadOnly();
        }

        public void AddClip(Guid trackId, MediaClip clip)
        {
            var track = _tracks.Find(t => t.Id == trackId);
            if (track == null)
                throw new ArgumentException($"Track with ID {trackId} not found", nameof(trackId));

            track.AddClip(clip);
            OnTimelineChanged(new TimelineChangedEventArgs(
                TimelineChangedEventArgs.ChangeType.ClipAdded, clip.Id));
        }

        public bool RemoveClip(Guid clipId)
        {
            foreach (var track in _tracks)
            {
                if (track.RemoveClip(clipId))
                {
                    OnTimelineChanged(new TimelineChangedEventArgs(
                        TimelineChangedEventArgs.ChangeType.ClipRemoved, clipId));
                    return true;
                }
            }
            return false;
        }

        public IReadOnlyList<MediaClip> GetClipsAtPosition(TimePosition position)
        {
            var clips = new List<MediaClip>();
            foreach (var track in _tracks)
            {
                var clip = track.GetClipAt(position);
                if (clip != null)
                    clips.Add(clip);
            }
            return clips.AsReadOnly();
        }

        public Duration GetTotalDuration()
        {
            var maxDuration = Duration.Zero;
            foreach (var track in _tracks)
            {
                var trackDuration = track.GetTotalDuration();
                if (trackDuration > maxDuration)
                    maxDuration = trackDuration;
            }
            return maxDuration;
        }

        public void MoveClip(Guid clipId, TimePosition newStartPosition)
        {
            foreach (var track in _tracks)
            {
                var clip = track.Clips.FirstOrDefault(c => c.Id == clipId);
                if (clip != null)
                {
                    track.MoveClip(clipId, newStartPosition);
                    OnTimelineChanged(new TimelineChangedEventArgs(
                        TimelineChangedEventArgs.ChangeType.ClipMoved, clipId));
                    return;
                }
            }
            throw new ArgumentException($"Clip with ID {clipId} not found", nameof(clipId));
        }

        public TimelineTrack? GetTrack(Guid trackId)
        {
            return _tracks.Find(t => t.Id == trackId);
        }

        public MediaClip? GetClip(Guid clipId)
        {
            foreach (var track in _tracks)
            {
                var clip = track.Clips.FirstOrDefault(c => c.Id == clipId);
                if (clip != null) return clip;
            }
            return null;
        }

        private void OnTimelineChanged(TimelineChangedEventArgs args)
        {
            TimelineChanged?.Invoke(this, args);
        }
    }
}