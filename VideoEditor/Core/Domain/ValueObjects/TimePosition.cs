using System;

namespace VideoEditor.Core.Domain.ValueObjects
{
    /// <summary>
    /// Represents a time position in the timeline with high precision
    /// </summary>
    public readonly struct TimePosition : IComparable<TimePosition>, IEquatable<TimePosition>
    {
        private readonly long _ticks;

        public TimePosition(TimeSpan timeSpan) : this(timeSpan.Ticks) { }

        public TimePosition(long ticks)
        {
            _ticks = ticks;
        }

        public TimePosition(double seconds) : this(TimeSpan.FromSeconds(seconds).Ticks) { }

        public static TimePosition Zero => new(0);
        
        public static TimePosition FromMilliseconds(double milliseconds) => new(TimeSpan.FromMilliseconds(milliseconds).Ticks);
        
        public static TimePosition FromSeconds(double seconds) => new(TimeSpan.FromSeconds(seconds).Ticks);
        
        public static TimePosition FromMinutes(double minutes) => new(TimeSpan.FromMinutes(minutes).Ticks);

        public long Ticks => _ticks;
        
        public double TotalMilliseconds => TimeSpan.FromTicks(_ticks).TotalMilliseconds;
        
        public double TotalSeconds => TimeSpan.FromTicks(_ticks).TotalSeconds;
        
        public double TotalMinutes => TimeSpan.FromTicks(_ticks).TotalMinutes;

        public TimeSpan ToTimeSpan() => TimeSpan.FromTicks(_ticks);

        public static TimePosition operator +(TimePosition left, TimePosition right) => new(left._ticks + right._ticks);
        
        public static TimePosition operator -(TimePosition left, TimePosition right) => new(left._ticks - right._ticks);
        
        public static TimePosition operator *(TimePosition time, double multiplier) => new((long)(time._ticks * multiplier));
        
        public static TimePosition operator /(TimePosition time, double divisor) => new((long)(time._ticks / divisor));

        public static bool operator ==(TimePosition left, TimePosition right) => left._ticks == right._ticks;
        
        public static bool operator !=(TimePosition left, TimePosition right) => left._ticks != right._ticks;
        
        public static bool operator <(TimePosition left, TimePosition right) => left._ticks < right._ticks;
        
        public static bool operator >(TimePosition left, TimePosition right) => left._ticks > right._ticks;
        
        public static bool operator <=(TimePosition left, TimePosition right) => left._ticks <= right._ticks;
        
        public static bool operator >=(TimePosition left, TimePosition right) => left._ticks >= right._ticks;

        public int CompareTo(TimePosition other) => _ticks.CompareTo(other._ticks);

        public bool Equals(TimePosition other) => _ticks == other._ticks;

        public override bool Equals(object? obj) => obj is TimePosition other && Equals(other);

        public override int GetHashCode() => _ticks.GetHashCode();

        public override string ToString() => $"{TotalSeconds:F3}s";
    }
}