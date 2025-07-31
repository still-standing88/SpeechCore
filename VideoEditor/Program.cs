using System;
using System.Threading.Tasks;
using VideoEditor.Core.Application.Services;
using VideoEditor.Core.Domain.Entities;
using VideoEditor.Core.Domain.ValueObjects;
using VideoEditor.Core.Infrastructure.Logging;
using VideoEditor.Core.Infrastructure.Media;

namespace VideoEditor
{
    /// <summary>
    /// Console application demonstrating the Video Editor architecture
    /// This serves as a foundation that can be extended with a full GUI
    /// </summary>
    public class Program
    {
        private static VideoEditorLogger? _logger;

        public static async Task Main(string[] args)
        {
            try
            {
                Console.WriteLine("=== Video Editor Core Architecture Demo ===");
                Console.WriteLine();

                // Initialize logging
                InitializeLogging();
                _logger?.LogInformation("Video Editor console demo starting");

                // Demonstrate core architecture
                await DemonstrateArchitecture();

                _logger?.LogInformation("Video Editor console demo completed");
                Console.WriteLine("\nPress any key to exit...");
                Console.ReadKey();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Fatal error: {ex.Message}");
                _logger?.LogCritical("Fatal error in main", ex);
                Environment.Exit(1);
            }
            finally
            {
                _logger?.Dispose();
            }
        }

        private static void InitializeLogging()
        {
            _logger = new VideoEditorLogger(LogLevel.Information);
            _logger.AddOutput(new ConsoleLogOutput());

            var logPath = System.IO.Path.Combine(Environment.CurrentDirectory, "VideoEditor.log");
            _logger.AddOutput(new FileLogOutput(logPath));

            Console.WriteLine($"Logging initialized. Log file: {logPath}");
        }

        private static async Task DemonstrateArchitecture()
        {
            if (_logger == null) return;

            Console.WriteLine("1. Creating services...");

            // Initialize core services
            var projectService = new ProjectService();
            var timelineService = new TimelineService();
            var mediaEngine = new WindowsMediaEngine(_logger);

            // Initialize media engine
            var engineInitialized = await mediaEngine.InitializeAsync();
            Console.WriteLine($"   Media engine initialized: {engineInitialized}");

            Console.WriteLine("\n2. Creating sample project...");

            // Create a sample project
            var project = projectService.CreateNew("Demo Project");
            project.Settings.VideoWidth = 1920;
            project.Settings.VideoHeight = 1080;
            project.Settings.FrameRate = 30.0;

            Console.WriteLine($"   Project created: {project.Name}");
            Console.WriteLine($"   Settings: {project.Settings}");

            Console.WriteLine("\n3. Adding tracks to timeline...");

            // Create sample tracks
            var videoTrack = new TimelineTrack(TrackType.Video, "Main Video Track");
            var audioTrack = new TimelineTrack(TrackType.Audio, "Main Audio Track");

            timelineService.AddTrack(videoTrack);
            timelineService.AddTrack(audioTrack);

            Console.WriteLine($"   Added {timelineService.GetTracks().Count} tracks");

            Console.WriteLine("\n4. Adding sample media clips...");

            // Create sample media clips
            var videoClip = new MediaClip(
                "/path/to/sample/video.mp4",
                MediaType.Video,
                TimePosition.FromSeconds(0),
                Duration.FromSeconds(30),
                Duration.FromSeconds(60),
                "Sample Video"
            );

            var audioClip = new MediaClip(
                "/path/to/sample/audio.mp3",
                MediaType.Audio,
                TimePosition.FromSeconds(0),
                Duration.FromSeconds(25),
                Duration.FromSeconds(45),
                "Background Music"
            );

            // Add clips to tracks
            timelineService.AddClip(videoTrack.Id, videoClip);
            timelineService.AddClip(audioTrack.Id, audioClip);

            Console.WriteLine($"   Video clip: {videoClip}");
            Console.WriteLine($"   Audio clip: {audioClip}");

            Console.WriteLine("\n5. Testing timeline operations...");

            // Test timeline positioning
            timelineService.CurrentPosition = TimePosition.FromSeconds(10);
            var clipsAtPosition = timelineService.GetClipsAtPosition(TimePosition.FromSeconds(10));
            
            Console.WriteLine($"   Current position: {timelineService.CurrentPosition}");
            Console.WriteLine($"   Clips at position: {clipsAtPosition.Count}");

            var totalDuration = timelineService.GetTotalDuration();
            Console.WriteLine($"   Total timeline duration: {totalDuration}");

            Console.WriteLine("\n6. Testing media engine...");

            // Load project into media engine
            var projectLoaded = await mediaEngine.LoadProjectAsync(project);
            Console.WriteLine($"   Project loaded in media engine: {projectLoaded}");

            // Test playback controls
            Console.WriteLine("   Testing playback controls...");
            
            await mediaEngine.PlayAsync();
            Console.WriteLine($"   Playback state: {mediaEngine.State}");

            await Task.Delay(1000); // Simulate playback

            await mediaEngine.PauseAsync();
            Console.WriteLine($"   Playback state: {mediaEngine.State}");

            await mediaEngine.SeekAsync(TimePosition.FromSeconds(5));
            Console.WriteLine($"   Seeked to: {mediaEngine.CurrentPosition}");

            await mediaEngine.StopAsync();
            Console.WriteLine($"   Playback state: {mediaEngine.State}");

            Console.WriteLine("\n7. Architecture demonstration complete!");

            Console.WriteLine("\nArchitecture Summary:");
            Console.WriteLine("- Domain Layer: Core entities (Project, MediaClip, TimelineTrack)");
            Console.WriteLine("- Application Layer: Services (ProjectService, TimelineService)");
            Console.WriteLine("- Infrastructure Layer: Logging, Media Engine");
            Console.WriteLine("- Value Objects: TimePosition, Duration, TimeRange for precision");
            Console.WriteLine("- Clean separation of concerns with SOLID principles");

            // Cleanup
            mediaEngine.Dispose();
        }
    }
}