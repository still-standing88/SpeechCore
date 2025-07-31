using System;
using System.Collections.Generic;
using VideoEditor.Core.Domain.ValueObjects;

namespace VideoEditor.Core.Domain.Entities
{
    /// <summary>
    /// Represents a media clip on the timeline
    /// </summary>
    public class MediaClip
    {
        public Guid Id { get; }
        public string Name { get; set; }
        public string FilePath { get; }
        public MediaType MediaType { get; }
        public TimePosition StartPosition { get; set; }
        public Duration Duration { get; set; }
        public TimePosition MediaStartOffset { get; set; }
        public Duration MediaDuration { get; }
        public int ZIndex { get; set; }
        public bool IsLocked { get; set; }
        public bool IsMuted { get; set; }
        public double Volume { get; set; } = 1.0;
        public double Opacity { get; set; } = 1.0;
        
        private readonly List<IEffect> _effects = new();
        public IReadOnlyList<IEffect> Effects => _effects.AsReadOnly();

        public MediaClip(
            string filePath, 
            MediaType mediaType, 
            TimePosition startPosition, 
            Duration duration,
            Duration mediaDuration,
            string? name = null)
        {
            Id = Guid.NewGuid();
            FilePath = filePath ?? throw new ArgumentNullException(nameof(filePath));
            MediaType = mediaType;
            StartPosition = startPosition;
            Duration = duration;
            MediaDuration = mediaDuration;
            Name = name ?? System.IO.Path.GetFileNameWithoutExtension(filePath);
            MediaStartOffset = TimePosition.Zero;
        }

        public TimePosition EndPosition => StartPosition + new TimePosition(Duration.Ticks);

        public TimeRange TimeRange => new(StartPosition, EndPosition);

        public bool IsAtPosition(TimePosition position) => 
            position >= StartPosition && position < EndPosition;

        public void AddEffect(IEffect effect)
        {
            if (effect == null) throw new ArgumentNullException(nameof(effect));
            _effects.Add(effect);
        }

        public bool RemoveEffect(IEffect effect) => _effects.Remove(effect);

        public void ClearEffects() => _effects.Clear();

        public MediaClip Clone()
        {
            var clone = new MediaClip(FilePath, MediaType, StartPosition, Duration, MediaDuration, Name)
            {
                MediaStartOffset = MediaStartOffset,
                ZIndex = ZIndex,
                IsLocked = IsLocked,
                IsMuted = IsMuted,
                Volume = Volume,
                Opacity = Opacity
            };

            foreach (var effect in _effects)
            {
                clone.AddEffect(effect.Clone());
            }

            return clone;
        }

        public override string ToString() => $"{Name} [{TimeRange}]";
    }

    /// <summary>
    /// Base interface for all effects
    /// </summary>
    public interface IEffect
    {
        Guid Id { get; }
        string Name { get; }
        bool IsEnabled { get; set; }
        IEffect Clone();
    }
}