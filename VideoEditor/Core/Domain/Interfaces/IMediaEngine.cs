using System;
using System.Threading.Tasks;
using VideoEditor.Core.Domain.Entities;
using VideoEditor.Core.Domain.ValueObjects;

namespace VideoEditor.Core.Domain.Interfaces
{
    /// <summary>
    /// Core media engine interface for playback and preview
    /// </summary>
    public interface IMediaEngine : IDisposable
    {
        /// <summary>
        /// Initialize the media engine
        /// </summary>
        Task<bool> InitializeAsync();

        /// <summary>
        /// Load a project for playback
        /// </summary>
        Task<bool> LoadProjectAsync(Project project);

        /// <summary>
        /// Start playback from current position
        /// </summary>
        Task PlayAsync();

        /// <summary>
        /// Pause playback
        /// </summary>
        Task PauseAsync();

        /// <summary>
        /// Stop playback and return to beginning
        /// </summary>
        Task StopAsync();

        /// <summary>
        /// Seek to specific time position
        /// </summary>
        Task SeekAsync(TimePosition position);

        /// <summary>
        /// Current playback position
        /// </summary>
        TimePosition CurrentPosition { get; }

        /// <summary>
        /// Current playback state
        /// </summary>
        PlaybackState State { get; }

        /// <summary>
        /// Master volume level (0.0 to 1.0)
        /// </summary>
        double Volume { get; set; }

        /// <summary>
        /// Whether playback is muted
        /// </summary>
        bool IsMuted { get; set; }

        /// <summary>
        /// Current preview quality setting
        /// </summary>
        PreviewQuality PreviewQuality { get; set; }

        /// <summary>
        /// Event fired when playback state changes
        /// </summary>
        event EventHandler<PlaybackStateChangedEventArgs>? PlaybackStateChanged;

        /// <summary>
        /// Event fired when position changes during playback
        /// </summary>
        event EventHandler<PositionChangedEventArgs>? PositionChanged;
    }

    /// <summary>
    /// Event arguments for playback state changes
    /// </summary>
    public class PlaybackStateChangedEventArgs : EventArgs
    {
        public PlaybackState PreviousState { get; }
        public PlaybackState NewState { get; }

        public PlaybackStateChangedEventArgs(PlaybackState previousState, PlaybackState newState)
        {
            PreviousState = previousState;
            NewState = newState;
        }
    }

    /// <summary>
    /// Event arguments for position changes
    /// </summary>
    public class PositionChangedEventArgs : EventArgs
    {
        public TimePosition Position { get; }
        public bool IsUserInitiated { get; }

        public PositionChangedEventArgs(TimePosition position, bool isUserInitiated = false)
        {
            Position = position;
            IsUserInitiated = isUserInitiated;
        }
    }
}