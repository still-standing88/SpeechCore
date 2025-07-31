using System;

namespace VideoEditor.Core.Domain.ValueObjects
{
    /// <summary>
    /// Represents a time range from start to end position
    /// </summary>
    public readonly struct TimeRange : IEquatable<TimeRange>
    {
        public TimePosition Start { get; }
        public TimePosition End { get; }

        public TimeRange(TimePosition start, TimePosition end)
        {
            if (end < start)
                throw new ArgumentException("End time cannot be before start time");
            
            Start = start;
            End = end;
        }

        public TimeRange(TimePosition start, Duration duration)
        {
            Start = start;
            End = start + new TimePosition(duration.Ticks);
        }

        public Duration Duration => new((End - Start).Ticks);

        public bool Contains(TimePosition position) => position >= Start && position <= End;

        public bool Overlaps(TimeRange other) => Start < other.End && End > other.Start;

        public TimeRange? GetIntersection(TimeRange other)
        {
            if (!Overlaps(other))
                return null;

            var intersectionStart = Start > other.Start ? Start : other.Start;
            var intersectionEnd = End < other.End ? End : other.End;

            return new TimeRange(intersectionStart, intersectionEnd);
        }

        public TimeRange Union(TimeRange other)
        {
            var unionStart = Start < other.Start ? Start : other.Start;
            var unionEnd = End > other.End ? End : other.End;
            return new TimeRange(unionStart, unionEnd);
        }

        public static bool operator ==(TimeRange left, TimeRange right) => 
            left.Start == right.Start && left.End == right.End;
        
        public static bool operator !=(TimeRange left, TimeRange right) => !(left == right);

        public bool Equals(TimeRange other) => Start.Equals(other.Start) && End.Equals(other.End);

        public override bool Equals(object? obj) => obj is TimeRange other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(Start, End);

        public override string ToString() => $"[{Start} - {End}] ({Duration})";
    }
}