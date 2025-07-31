using System;
using System.Collections.Generic;
using VideoEditor.Core.Domain.Entities;
using VideoEditor.Core.Domain.ValueObjects;

namespace VideoEditor.Core.Domain.Interfaces
{
    /// <summary>
    /// Interface for timeline management
    /// </summary>
    public interface ITimelineManager
    {
        /// <summary>
        /// Add a track to the timeline
        /// </summary>
        void AddTrack(TimelineTrack track);

        /// <summary>
        /// Remove a track from the timeline
        /// </summary>
        bool RemoveTrack(Guid trackId);

        /// <summary>
        /// Get all tracks
        /// </summary>
        IReadOnlyList<TimelineTrack> GetTracks();

        /// <summary>
        /// Get tracks by type
        /// </summary>
        IReadOnlyList<TimelineTrack> GetTracksByType(TrackType type);

        /// <summary>
        /// Add a clip to a specific track
        /// </summary>
        void AddClip(Guid trackId, MediaClip clip);

        /// <summary>
        /// Remove a clip from the timeline
        /// </summary>
        bool RemoveClip(Guid clipId);

        /// <summary>
        /// Get all clips at a specific time position
        /// </summary>
        IReadOnlyList<MediaClip> GetClipsAtPosition(TimePosition position);

        /// <summary>
        /// Get the total duration of the timeline
        /// </summary>
        Duration GetTotalDuration();

        /// <summary>
        /// Current playhead position
        /// </summary>
        TimePosition CurrentPosition { get; set; }

        /// <summary>
        /// Event fired when the timeline changes
        /// </summary>
        event EventHandler<TimelineChangedEventArgs>? TimelineChanged;
    }

    /// <summary>
    /// Event arguments for timeline changes
    /// </summary>
    public class TimelineChangedEventArgs : EventArgs
    {
        public enum ChangeType
        {
            TrackAdded,
            TrackRemoved,
            ClipAdded,
            ClipRemoved,
            ClipMoved,
            PositionChanged
        }

        public ChangeType Type { get; }
        public Guid? EntityId { get; }

        public TimelineChangedEventArgs(ChangeType type, Guid? entityId = null)
        {
            Type = type;
            EntityId = entityId;
        }
    }
}