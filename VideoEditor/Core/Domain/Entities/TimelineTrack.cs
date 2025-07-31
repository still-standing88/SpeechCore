using System;
using System.Collections.Generic;
using System.Linq;
using VideoEditor.Core.Domain.ValueObjects;

namespace VideoEditor.Core.Domain.Entities
{
    /// <summary>
    /// Represents a track on the timeline that contains media clips
    /// </summary>
    public class TimelineTrack
    {
        public Guid Id { get; }
        public string Name { get; set; }
        public TrackType Type { get; }
        public int LayerIndex { get; set; }
        public bool IsVisible { get; set; } = true;
        public bool IsMuted { get; set; }
        public bool IsLocked { get; set; }
        public double Volume { get; set; } = 1.0;
        public double Opacity { get; set; } = 1.0;

        private readonly List<MediaClip> _clips = new();
        public IReadOnlyList<MediaClip> Clips => _clips.AsReadOnly();

        public TimelineTrack(TrackType type, string? name = null, int layerIndex = 0)
        {
            Id = Guid.NewGuid();
            Type = type;
            Name = name ?? $"{type} Track";
            LayerIndex = layerIndex;
        }

        public void AddClip(MediaClip clip)
        {
            if (clip == null) throw new ArgumentNullException(nameof(clip));
            
            // Check for overlaps with existing clips
            var overlappingClip = _clips.FirstOrDefault(c => c.TimeRange.Overlaps(clip.TimeRange));
            if (overlappingClip != null)
            {
                throw new InvalidOperationException(
                    $"Clip overlaps with existing clip '{overlappingClip.Name}' at {overlappingClip.TimeRange}");
            }

            _clips.Add(clip);
            _clips.Sort((a, b) => a.StartPosition.CompareTo(b.StartPosition));
        }

        public bool RemoveClip(MediaClip clip)
        {
            return _clips.Remove(clip);
        }

        public bool RemoveClip(Guid clipId)
        {
            var clip = _clips.FirstOrDefault(c => c.Id == clipId);
            return clip != null && _clips.Remove(clip);
        }

        public MediaClip? GetClipAt(TimePosition position)
        {
            return _clips.FirstOrDefault(c => c.IsAtPosition(position));
        }

        public IEnumerable<MediaClip> GetClipsInRange(TimeRange range)
        {
            return _clips.Where(c => c.TimeRange.Overlaps(range));
        }

        public Duration GetTotalDuration()
        {
            if (!_clips.Any())
                return Duration.Zero;

            var lastClip = _clips.OrderBy(c => c.EndPosition).Last();
            return new Duration(lastClip.EndPosition.Ticks);
        }

        public bool CanAddClip(MediaClip clip)
        {
            return !_clips.Any(c => c.TimeRange.Overlaps(clip.TimeRange));
        }

        public void MoveClip(Guid clipId, TimePosition newStartPosition)
        {
            var clip = _clips.FirstOrDefault(c => c.Id == clipId);
            if (clip == null)
                throw new ArgumentException($"Clip with ID {clipId} not found", nameof(clipId));

            var newTimeRange = new TimeRange(newStartPosition, clip.Duration);
            var overlappingClip = _clips.FirstOrDefault(c => c.Id != clipId && c.TimeRange.Overlaps(newTimeRange));
            
            if (overlappingClip != null)
            {
                throw new InvalidOperationException(
                    $"Cannot move clip to {newStartPosition} - would overlap with '{overlappingClip.Name}'");
            }

            clip.StartPosition = newStartPosition;
            _clips.Sort((a, b) => a.StartPosition.CompareTo(b.StartPosition));
        }

        public override string ToString() => $"{Name} ({Type}) - {_clips.Count} clips";
    }
}