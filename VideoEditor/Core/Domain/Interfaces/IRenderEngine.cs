using System;
using System.Threading.Tasks;
using VideoEditor.Core.Domain.Entities;
using VideoEditor.Core.Domain.ValueObjects;

namespace VideoEditor.Core.Domain.Interfaces
{
    /// <summary>
    /// Interface for rendering video frames and audio samples
    /// </summary>
    public interface IRenderEngine : IDisposable
    {
        /// <summary>
        /// Initialize the render engine with project settings
        /// </summary>
        Task<bool> InitializeAsync(ProjectSettings settings);

        /// <summary>
        /// Render a single frame at the specified time position
        /// </summary>
        Task<RenderFrame?> RenderFrameAsync(Project project, TimePosition position, PreviewQuality quality = PreviewQuality.Full);

        /// <summary>
        /// Render audio samples for the specified time range
        /// </summary>
        Task<AudioSamples?> RenderAudioAsync(Project project, TimeRange range);

        /// <summary>
        /// Export the entire project to a video file
        /// </summary>
        Task<bool> ExportAsync(Project project, string outputPath, ExportSettings settings, IProgress<ExportProgress>? progress = null);

        /// <summary>
        /// Current project settings
        /// </summary>
        ProjectSettings? Settings { get; }

        /// <summary>
        /// Whether hardware acceleration is available
        /// </summary>
        bool IsHardwareAccelerated { get; }
    }

    /// <summary>
    /// Represents a rendered video frame
    /// </summary>
    public class RenderFrame
    {
        public byte[] PixelData { get; }
        public int Width { get; }
        public int Height { get; }
        public TimePosition Timestamp { get; }

        public RenderFrame(byte[] pixelData, int width, int height, TimePosition timestamp)
        {
            PixelData = pixelData ?? throw new ArgumentNullException(nameof(pixelData));
            Width = width;
            Height = height;
            Timestamp = timestamp;
        }
    }

    /// <summary>
    /// Represents rendered audio samples
    /// </summary>
    public class AudioSamples
    {
        public float[] SampleData { get; }
        public int SampleRate { get; }
        public int Channels { get; }
        public TimeRange TimeRange { get; }

        public AudioSamples(float[] sampleData, int sampleRate, int channels, TimeRange timeRange)
        {
            SampleData = sampleData ?? throw new ArgumentNullException(nameof(sampleData));
            SampleRate = sampleRate;
            Channels = channels;
            TimeRange = timeRange;
        }
    }

    /// <summary>
    /// Export settings for video output
    /// </summary>
    public class ExportSettings
    {
        public string OutputFormat { get; set; } = "MP4";
        public string VideoCodec { get; set; } = "H.264";
        public string AudioCodec { get; set; } = "AAC";
        public int VideoBitrate { get; set; } = 8000; // kbps
        public int AudioBitrate { get; set; } = 192; // kbps
        public PreviewQuality Quality { get; set; } = PreviewQuality.Full;
        public bool UseHardwareAcceleration { get; set; } = true;

        public static ExportSettings Default => new();
    }

    /// <summary>
    /// Progress information for export operations
    /// </summary>
    public class ExportProgress
    {
        public TimePosition CurrentPosition { get; }
        public Duration TotalDuration { get; }
        public double ProgressPercentage { get; }
        public string? CurrentOperation { get; }

        public ExportProgress(TimePosition currentPosition, Duration totalDuration, string? currentOperation = null)
        {
            CurrentPosition = currentPosition;
            TotalDuration = totalDuration;
            ProgressPercentage = totalDuration.Ticks > 0 ? (currentPosition.Ticks / (double)totalDuration.Ticks) * 100.0 : 0.0;
            CurrentOperation = currentOperation;
        }
    }
}