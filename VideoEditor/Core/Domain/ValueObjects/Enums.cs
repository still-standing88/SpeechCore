namespace VideoEditor.Core.Domain.ValueObjects
{
    /// <summary>
    /// Supported media types for clips
    /// </summary>
    public enum MediaType
    {
        Unknown,
        Video,
        Audio,
        Image,
        Text
    }
}

namespace VideoEditor.Core.Domain.ValueObjects
{
    /// <summary>
    /// Track types for the timeline
    /// </summary>
    public enum TrackType
    {
        Video,
        Audio,
        Text,
        Effect
    }
}

namespace VideoEditor.Core.Domain.ValueObjects
{
    /// <summary>
    /// Playback states for the media engine
    /// </summary>
    public enum PlaybackState
    {
        Stopped,
        Playing,
        Paused,
        Loading,
        Buffering
    }
}

namespace VideoEditor.Core.Domain.ValueObjects
{
    /// <summary>
    /// Quality settings for preview rendering
    /// </summary>
    public enum PreviewQuality
    {
        Quarter = 25,
        Half = 50,
        Full = 100
    }
}