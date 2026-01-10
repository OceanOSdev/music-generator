#pragma once
#ifndef PITCH_HELPERS_HPP
#define PITCH_HELPERS_HPP

namespace Adapter {
using Note = double;

// Notes

/// A4 = 440hz
inline constexpr Note A = 440;

/// B4 = 493.8833hz
inline constexpr Note B = 493.8833;

/// C4 = 391.9954hz
inline constexpr Note C = 261.6256;

/// D4 = 293.6648hz
inline constexpr Note D = 293.6648;

/// E4 = 329.6276hz
inline constexpr Note E = 329.6276;

/// F4 = 349.2282hz
inline constexpr Note F = 349.2282;

/// G4 = 391.9954hz
inline constexpr Note G = 391.9954;

/// A#4 = 466.1638hz;
inline constexpr Note ASharp = 466.1638;

/// Bb4 = 466.1638hz;
inline constexpr Note BFlat = ASharp;

/// C#4 = 277.1826hz;
inline constexpr Note CSharp = 277.1826;

/// Db4 = 277.1826hz;
inline constexpr Note DFlat = CSharp;

/// D#4 = 311.1270hz;
inline constexpr Note DSharp = 311.1270;

/// Eb4 = 311.1270hz;
inline constexpr Note EFlat = DSharp;

/// F#4 = 369.9944hz
inline constexpr Note FSharp = 369.9944;

/// Gb4 = 369.9944hz
inline constexpr Note GFlat = FSharp;

/// G#4 = 415.3047hz
inline constexpr Note GSharp = 415.3047;

/// Ab4 = 415.3047hz
inline constexpr Note AFlat = GSharp;

/// Given a [Note] note (assumed to be in the fourth octave)
/// returns the frequence of the note in the given octave.
Note get_freq(Note note, unsigned int octave = 4);

} // namespace Adapter

#endif
