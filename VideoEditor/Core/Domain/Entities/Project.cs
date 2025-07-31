using System;
using System.Collections.Generic;
using VideoEditor.Core.Domain.ValueObjects;

namespace VideoEditor.Core.Domain.Entities
{
    /// <summary>
    /// Represents a video editing project
    /// </summary>
    public class Project
    {
        public Guid Id { get; }
        public string Name { get; set; }
        public string? FilePath { get; set; }
        public DateTime CreatedAt { get; }
        public DateTime LastModified { get; set; }
        public ProjectSettings Settings { get; set; }
        
        private readonly List<TimelineTrack> _tracks = new();
        public IReadOnlyList<TimelineTrack> Tracks => _tracks.AsReadOnly();

        private readonly List<MediaAsset> _assets = new();
        public IReadOnlyList<MediaAsset> Assets => _assets.AsReadOnly();

        public Project(string name, ProjectSettings? settings = null)
        {
            Id = Guid.NewGuid();
            Name = name ?? throw new ArgumentNullException(nameof(name));
            CreatedAt = DateTime.UtcNow;
            LastModified = CreatedAt;
            Settings = settings ?? ProjectSettings.Default;
        }

        public void AddTrack(TimelineTrack track)
        {
            if (track == null) throw new ArgumentNullException(nameof(track));
            _tracks.Add(track);
            LastModified = DateTime.UtcNow;
        }

        public bool RemoveTrack(Guid trackId)
        {
            var track = _tracks.Find(t => t.Id == trackId);
            if (track != null && _tracks.Remove(track))
            {
                LastModified = DateTime.UtcNow;
                return true;
            }
            return false;
        }

        public void AddAsset(MediaAsset asset)
        {
            if (asset == null) throw new ArgumentNullException(nameof(asset));
            _assets.Add(asset);
            LastModified = DateTime.UtcNow;
        }

        public bool RemoveAsset(Guid assetId)
        {
            var asset = _assets.Find(a => a.Id == assetId);
            if (asset != null && _assets.Remove(asset))
            {
                LastModified = DateTime.UtcNow;
                return true;
            }
            return false;
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

        public void MarkAsModified()
        {
            LastModified = DateTime.UtcNow;
        }

        public override string ToString() => $"{Name} - {_tracks.Count} tracks, {_assets.Count} assets";
    }

    /// <summary>
    /// Project settings and configuration
    /// </summary>
    public class ProjectSettings
    {
        public int VideoWidth { get; set; } = 1920;
        public int VideoHeight { get; set; } = 1080;
        public double FrameRate { get; set; } = 30.0;
        public int AudioSampleRate { get; set; } = 48000;
        public int AudioChannels { get; set; } = 2;
        public int AudioBitsPerSample { get; set; } = 16;
        public PreviewQuality DefaultPreviewQuality { get; set; } = PreviewQuality.Half;
        public bool AutoSaveEnabled { get; set; } = true;
        public int AutoSaveIntervalMinutes { get; set; } = 5;

        public static ProjectSettings Default => new();

        public override string ToString() => 
            $"{VideoWidth}x{VideoHeight}@{FrameRate}fps, {AudioSampleRate}Hz {AudioChannels}ch";
    }

    /// <summary>
    /// Represents a media asset imported into the project
    /// </summary>
    public class MediaAsset
    {
        public Guid Id { get; }
        public string Name { get; set; }
        public string FilePath { get; }
        public MediaType MediaType { get; }
        public Duration Duration { get; }
        public long FileSizeBytes { get; }
        public DateTime ImportedAt { get; }
        public string? ThumbnailPath { get; set; }
        public Dictionary<string, object> Metadata { get; }

        public MediaAsset(
            string filePath, 
            MediaType mediaType, 
            Duration duration, 
            long fileSizeBytes,
            string? name = null)
        {
            Id = Guid.NewGuid();
            FilePath = filePath ?? throw new ArgumentNullException(nameof(filePath));
            MediaType = mediaType;
            Duration = duration;
            FileSizeBytes = fileSizeBytes;
            ImportedAt = DateTime.UtcNow;
            Name = name ?? System.IO.Path.GetFileNameWithoutExtension(filePath);
            Metadata = new Dictionary<string, object>();
        }

        public override string ToString() => $"{Name} ({MediaType}) - {Duration}";
    }
}