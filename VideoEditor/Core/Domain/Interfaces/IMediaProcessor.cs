using System;
using System.Threading.Tasks;
using VideoEditor.Core.Domain.ValueObjects;

namespace VideoEditor.Core.Domain.Interfaces
{
    /// <summary>
    /// Base interface for all media processors
    /// </summary>
    public interface IMediaProcessor : IDisposable
    {
        /// <summary>
        /// Initialize the processor with media file
        /// </summary>
        Task<bool> InitializeAsync(string filePath);

        /// <summary>
        /// Get the duration of the media
        /// </summary>
        Duration GetDuration();

        /// <summary>
        /// Check if the media is loaded and ready
        /// </summary>
        bool IsLoaded { get; }

        /// <summary>
        /// Get the media type
        /// </summary>
        MediaType MediaType { get; }
    }
}

namespace VideoEditor.Core.Domain.Interfaces
{
    /// <summary>
    /// Interface for video processing capabilities
    /// </summary>
    public interface IVideoProcessor : IMediaProcessor
    {
        /// <summary>
        /// Get video frame at specific time position
        /// </summary>
        Task<byte[]?> GetFrameAsync(TimePosition position);

        /// <summary>
        /// Get video width in pixels
        /// </summary>
        int Width { get; }

        /// <summary>
        /// Get video height in pixels
        /// </summary>
        int Height { get; }

        /// <summary>
        /// Get video frame rate
        /// </summary>
        double FrameRate { get; }
    }
}

namespace VideoEditor.Core.Domain.Interfaces
{
    /// <summary>
    /// Interface for audio processing capabilities
    /// </summary>
    public interface IAudioProcessor : IMediaProcessor
    {
        /// <summary>
        /// Get audio samples for a time range
        /// </summary>
        Task<float[]?> GetSamplesAsync(TimeRange range);

        /// <summary>
        /// Get audio sample rate
        /// </summary>
        int SampleRate { get; }

        /// <summary>
        /// Get number of audio channels
        /// </summary>
        int Channels { get; }

        /// <summary>
        /// Get bits per sample
        /// </summary>
        int BitsPerSample { get; }
    }
}