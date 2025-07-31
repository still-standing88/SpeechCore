using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace VideoEditor.Core.Infrastructure.Logging
{
    /// <summary>
    /// Log levels for the application
    /// </summary>
    public enum LogLevel
    {
        Trace = 0,
        Debug = 1,
        Information = 2,
        Warning = 3,
        Error = 4,
        Critical = 5
    }

    /// <summary>
    /// Interface for logging providers
    /// </summary>
    public interface ILogger
    {
        void Log(LogLevel level, string message, Exception? exception = null);
        void Log(LogLevel level, string message, params object[] args);
        void LogTrace(string message, params object[] args);
        void LogDebug(string message, params object[] args);
        void LogInformation(string message, params object[] args);
        void LogWarning(string message, params object[] args);
        void LogError(string message, Exception? exception = null, params object[] args);
        void LogCritical(string message, Exception? exception = null, params object[] args);
    }

    /// <summary>
    /// Custom logging implementation for the video editor
    /// </summary>
    public class VideoEditorLogger : ILogger, IDisposable
    {
        private readonly LogLevel _minimumLevel;
        private readonly List<ILogOutput> _outputs = new();
        private readonly object _lock = new();
        private bool _disposed;

        public VideoEditorLogger(LogLevel minimumLevel = LogLevel.Information)
        {
            _minimumLevel = minimumLevel;
        }

        public void AddOutput(ILogOutput output)
        {
            lock (_lock)
            {
                _outputs.Add(output);
            }
        }

        public void RemoveOutput(ILogOutput output)
        {
            lock (_lock)
            {
                _outputs.Remove(output);
            }
        }

        public void Log(LogLevel level, string message, Exception? exception = null)
        {
            if (level < _minimumLevel || _disposed) return;

            var logEntry = new LogEntry
            {
                Timestamp = DateTime.UtcNow,
                Level = level,
                Message = message,
                Exception = exception,
                ThreadId = Thread.CurrentThread.ManagedThreadId
            };

            lock (_lock)
            {
                foreach (var output in _outputs)
                {
                    try
                    {
                        output.WriteLog(logEntry);
                    }
                    catch
                    {
                        // Ignore output failures to prevent logging from crashing the app
                    }
                }
            }
        }

        public void Log(LogLevel level, string message, params object[] args)
        {
            if (level < _minimumLevel || _disposed) return;
            Log(level, string.Format(message, args));
        }

        public void LogTrace(string message, params object[] args) => Log(LogLevel.Trace, message, args);
        public void LogDebug(string message, params object[] args) => Log(LogLevel.Debug, message, args);
        public void LogInformation(string message, params object[] args) => Log(LogLevel.Information, message, args);
        public void LogWarning(string message, params object[] args) => Log(LogLevel.Warning, message, args);
        
        public void LogError(string message, Exception? exception = null, params object[] args)
        {
            Log(LogLevel.Error, args.Length > 0 ? string.Format(message, args) : message, exception);
        }
        
        public void LogCritical(string message, Exception? exception = null, params object[] args)
        {
            Log(LogLevel.Critical, args.Length > 0 ? string.Format(message, args) : message, exception);
        }

        public void Dispose()
        {
            if (!_disposed)
            {
                lock (_lock)
                {
                    foreach (var output in _outputs)
                    {
                        if (output is IDisposable disposable)
                            disposable.Dispose();
                    }
                    _outputs.Clear();
                }
                _disposed = true;
            }
        }
    }

    /// <summary>
    /// Represents a log entry
    /// </summary>
    public class LogEntry
    {
        public DateTime Timestamp { get; set; }
        public LogLevel Level { get; set; }
        public string Message { get; set; } = string.Empty;
        public Exception? Exception { get; set; }
        public int ThreadId { get; set; }
    }

    /// <summary>
    /// Interface for log output destinations
    /// </summary>
    public interface ILogOutput
    {
        void WriteLog(LogEntry entry);
    }

    /// <summary>
    /// Console log output implementation
    /// </summary>
    public class ConsoleLogOutput : ILogOutput
    {
        public void WriteLog(LogEntry entry)
        {
            var message = FormatLogEntry(entry);
            
            // Use different colors for different log levels
            var originalColor = Console.ForegroundColor;
            Console.ForegroundColor = entry.Level switch
            {
                LogLevel.Trace => ConsoleColor.Gray,
                LogLevel.Debug => ConsoleColor.Blue,
                LogLevel.Information => ConsoleColor.White,
                LogLevel.Warning => ConsoleColor.Yellow,
                LogLevel.Error => ConsoleColor.Red,
                LogLevel.Critical => ConsoleColor.Magenta,
                _ => ConsoleColor.White
            };

            Console.WriteLine(message);
            Console.ForegroundColor = originalColor;
        }

        private static string FormatLogEntry(LogEntry entry)
        {
            var sb = new StringBuilder();
            sb.Append($"[{entry.Timestamp:yyyy-MM-dd HH:mm:ss.fff}] ");
            sb.Append($"[{entry.Level.ToString().ToUpper()}] ");
            sb.Append($"[T{entry.ThreadId:D2}] ");
            sb.Append(entry.Message);

            if (entry.Exception != null)
            {
                sb.AppendLine();
                sb.Append("Exception: ");
                sb.Append(entry.Exception.ToString());
            }

            return sb.ToString();
        }
    }

    /// <summary>
    /// File log output implementation
    /// </summary>
    public class FileLogOutput : ILogOutput, IDisposable
    {
        private readonly string _filePath;
        private readonly StreamWriter _writer;
        private readonly object _lock = new();
        private bool _disposed;

        public FileLogOutput(string filePath)
        {
            _filePath = filePath;
            var directory = Path.GetDirectoryName(filePath);
            if (!string.IsNullOrEmpty(directory) && !Directory.Exists(directory))
                Directory.CreateDirectory(directory);

            _writer = new StreamWriter(filePath, append: true, Encoding.UTF8) { AutoFlush = true };
        }

        public void WriteLog(LogEntry entry)
        {
            if (_disposed) return;

            var message = FormatLogEntry(entry);
            
            lock (_lock)
            {
                _writer?.WriteLine(message);
            }
        }

        private static string FormatLogEntry(LogEntry entry)
        {
            var sb = new StringBuilder();
            sb.Append($"[{entry.Timestamp:yyyy-MM-dd HH:mm:ss.fff}] ");
            sb.Append($"[{entry.Level}] ");
            sb.Append($"[T{entry.ThreadId:D2}] ");
            sb.Append(entry.Message);

            if (entry.Exception != null)
            {
                sb.AppendLine();
                sb.Append("Exception: ");
                sb.Append(entry.Exception.ToString());
            }

            return sb.ToString();
        }

        public void Dispose()
        {
            if (!_disposed)
            {
                lock (_lock)
                {
                    _writer?.Dispose();
                }
                _disposed = true;
            }
        }
    }
}