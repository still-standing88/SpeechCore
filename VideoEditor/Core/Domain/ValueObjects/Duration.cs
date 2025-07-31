using System;

namespace VideoEditor.Core.Domain.ValueObjects
{
    /// <summary>
    /// Represents a time duration with high precision
    /// </summary>
    public readonly struct Duration : IComparable<Duration>, IEquatable<Duration>
    {
        private readonly long _ticks;

        public Duration(TimeSpan timeSpan) : this(timeSpan.Ticks) { }

        public Duration(long ticks)
        {
            if (ticks < 0)
                throw new ArgumentException("Duration cannot be negative", nameof(ticks));
            _ticks = ticks;
        }

        public Duration(double seconds) : this(TimeSpan.FromSeconds(seconds).Ticks) { }

        public static Duration Zero => new(0);
        
        public static Duration FromMilliseconds(double milliseconds) => new(TimeSpan.FromMilliseconds(milliseconds).Ticks);
        
        public static Duration FromSeconds(double seconds) => new(TimeSpan.FromSeconds(seconds).Ticks);
        
        public static Duration FromMinutes(double minutes) => new(TimeSpan.FromMinutes(minutes).Ticks);

        public long Ticks => _ticks;
        
        public double TotalMilliseconds => TimeSpan.FromTicks(_ticks).TotalMilliseconds;
        
        public double TotalSeconds => TimeSpan.FromTicks(_ticks).TotalSeconds;
        
        public double TotalMinutes => TimeSpan.FromTicks(_ticks).TotalMinutes;

        public TimeSpan ToTimeSpan() => TimeSpan.FromTicks(_ticks);

        public static Duration operator +(Duration left, Duration right) => new(left._ticks + right._ticks);
        
        public static Duration operator -(Duration left, Duration right) => new(Math.Max(0, left._ticks - right._ticks));
        
        public static Duration operator *(Duration duration, double multiplier) => new((long)(duration._ticks * multiplier));
        
        public static Duration operator /(Duration duration, double divisor) => new((long)(duration._ticks / divisor));

        public static bool operator ==(Duration left, Duration right) => left._ticks == right._ticks;
        
        public static bool operator !=(Duration left, Duration right) => left._ticks != right._ticks;
        
        public static bool operator <(Duration left, Duration right) => left._ticks < right._ticks;
        
        public static bool operator >(Duration left, Duration right) => left._ticks > right._ticks;
        
        public static bool operator <=(Duration left, Duration right) => left._ticks <= right._ticks;
        
        public static bool operator >=(Duration left, Duration right) => left._ticks >= right._ticks;

        public int CompareTo(Duration other) => _ticks.CompareTo(other._ticks);

        public bool Equals(Duration other) => _ticks == other._ticks;

        public override bool Equals(object? obj) => obj is Duration other && Equals(other);

        public override int GetHashCode() => _ticks.GetHashCode();

        public override string ToString() => $"{TotalSeconds:F3}s";
    }
}