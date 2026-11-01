/* ============================================================================
 * Oasis Layer 1b — Procedural Solarpunk WebAudio Engine (Zero External Assets)
 * Provides gentle ambient breeze, pentatonic marimba pulses, daytime birdsong,
 * and tactile UI / stewardship sound effects.
 * ============================================================================ */
(function () {
  'use strict';

  const PENTATONIC_HZ = [261.63, 293.66, 329.63, 392.00, 440.00, 523.25, 587.33, 659.25];

  const OasisAudio = {
    ctx: null,
    masterGain: null,
    windGain: null,
    windFilter: null,
    enabled: false,
    timeWarp: 1,
    isDaytime: true,
    _ambientTimer: null,
    _birdTimer: null,

    _ensureContext() {
      if (!this.ctx) {
        const AudioCtx = window.AudioContext || window.webkitAudioContext;
        if (!AudioCtx) return false;
        this.ctx = new AudioCtx();
        this.masterGain = this.ctx.createGain();
        this.masterGain.gain.value = 0.0;
        this.masterGain.connect(this.ctx.destination);
        this._setupWindLayer();
      }
      if (this.ctx.state === 'suspended') {
        this.ctx.resume().catch(() => {});
      }
      return true;
    },

    _setupWindLayer() {
      if (!this.ctx || this.windGain) return;
      const sampleRate = this.ctx.sampleRate;
      const frameCount = sampleRate * 3;
      const buffer = this.ctx.createBuffer(1, frameCount, sampleRate);
      const data = buffer.getChannelData(0);

      // Paul Kellet pink-noise approximation for warm natural breeze
      let b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0;
      for (let i = 0; i < frameCount; i++) {
        const white = Math.random() * 2 - 1;
        b0 = 0.99886 * b0 + white * 0.0555179;
        b1 = 0.99332 * b1 + white * 0.0750759;
        b2 = 0.96900 * b2 + white * 0.1538520;
        b3 = 0.86650 * b3 + white * 0.3104856;
        b4 = 0.55000 * b4 + white * 0.5329522;
        b5 = -0.7616 * b5 - white * 0.0168980;
        data[i] = (b0 + b1 + b2 + b3 + b4 + b5 + b6 + white * 0.5362) * 0.035;
        b6 = white * 0.115926;
      }

      const source = this.ctx.createBufferSource();
      source.buffer = buffer;
      source.loop = true;

      this.windFilter = this.ctx.createBiquadFilter();
      this.windFilter.type = 'bandpass';
      this.windFilter.frequency.value = 360;
      this.windFilter.Q.value = 0.85;

      this.windGain = this.ctx.createGain();
      this.windGain.gain.value = 0.22;

      source.connect(this.windFilter);
      this.windFilter.connect(this.windGain);
      this.windGain.connect(this.masterGain);
      source.start();
    },

    _startSchedulers() {
      this._stopSchedulers();
      this._ambientTimer = window.setInterval(() => {
        if (!this.enabled || !this.ctx || this.timeWarp <= 0) return;
        if (this.windFilter) {
          const targetFreq = 280 + Math.random() * 240;
          this.windFilter.frequency.setTargetAtTime(targetFreq, this.ctx.currentTime, 1.4);
        }
        if (Math.random() < 0.68) {
          const freq = PENTATONIC_HZ[Math.floor(Math.random() * PENTATONIC_HZ.length)];
          this._playMarimbaNote(freq, 0.065);
        }
      }, 2600);

      this._birdTimer = window.setInterval(() => {
        if (!this.enabled || !this.ctx || this.timeWarp <= 0 || !this.isDaytime) return;
        if (Math.random() < 0.45) {
          this._playBirdChirp();
        }
      }, 4800);
    },

    _stopSchedulers() {
      if (this._ambientTimer) {
        clearInterval(this._ambientTimer);
        this._ambientTimer = null;
      }
      if (this._birdTimer) {
        clearInterval(this._birdTimer);
        this._birdTimer = null;
      }
    },

    _playMarimbaNote(freqHz, peakGain) {
      if (!this.enabled || !this.ctx) return;
      const now = this.ctx.currentTime;
      const osc = this.ctx.createOscillator();
      const overtone = this.ctx.createOscillator();
      const gain = this.ctx.createGain();

      osc.type = 'sine';
      osc.frequency.setValueAtTime(freqHz, now);

      overtone.type = 'triangle';
      overtone.frequency.setValueAtTime(freqHz * 2.0, now);

      const overtoneGain = this.ctx.createGain();
      overtoneGain.gain.setValueAtTime(0.18, now);
      overtoneGain.gain.exponentialRampToValueAtTime(0.001, now + 0.16);

      gain.gain.setValueAtTime(0.0001, now);
      gain.gain.linearRampToValueAtTime(peakGain || 0.06, now + 0.018);
      gain.gain.exponentialRampToValueAtTime(0.0001, now + 1.35);

      osc.connect(gain);
      overtone.connect(overtoneGain);
      overtoneGain.connect(gain);
      gain.connect(this.masterGain);

      osc.start(now);
      overtone.start(now);
      osc.stop(now + 1.4);
      overtone.stop(now + 0.22);
    },

    _playBirdChirp() {
      if (!this.enabled || !this.ctx) return;
      const now = this.ctx.currentTime;
      const base = 1900 + Math.random() * 850;
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();

      osc.type = 'sine';
      osc.frequency.setValueAtTime(base, now);
      osc.frequency.exponentialRampToValueAtTime(base * 1.28, now + 0.065);
      osc.frequency.exponentialRampToValueAtTime(base * 0.94, now + 0.14);

      gain.gain.setValueAtTime(0.0001, now);
      gain.gain.linearRampToValueAtTime(0.024, now + 0.02);
      gain.gain.exponentialRampToValueAtTime(0.0001, now + 0.16);

      osc.connect(gain);
      gain.connect(this.masterGain);
      osc.start(now);
      osc.stop(now + 0.18);
    },

    setEnabled(flag) {
      this.enabled = Boolean(flag);
      if (this.enabled) {
        if (!this._ensureContext()) {
          this.enabled = false;
          return false;
        }
        this.masterGain.gain.setTargetAtTime(0.75, this.ctx.currentTime, 0.15);
        this._startSchedulers();
        this.playClick();
      } else {
        if (this.ctx && this.masterGain) {
          this.masterGain.gain.setTargetAtTime(0.0, this.ctx.currentTime, 0.08);
        }
        this._stopSchedulers();
      }
      this.syncToggleButtons();
      return this.enabled;
    },

    toggle() {
      return this.setEnabled(!this.enabled);
    },

    setTimeWarp(mult) {
      const numeric = Number(mult);
      if (!Number.isNaN(numeric)) {
        this.timeWarp = numeric;
      }
    },

    setDayPhase(hour24) {
      const h = Number(hour24);
      if (!Number.isNaN(h)) {
        this.isDaytime = h >= 6 && h < 20;
      }
    },

    syncToggleButtons() {
      const btn = document.getElementById('btn-audio-toggle');
      if (btn) {
        btn.textContent = this.enabled ? '🔊 Audio' : '🔇 Audio';
        btn.classList.toggle('active', this.enabled);
      }
    },

    playClick() {
      if (!this.enabled || !this._ensureContext()) return;
      const now = this.ctx.currentTime;
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();
      osc.type = 'triangle';
      osc.frequency.setValueAtTime(620, now);
      osc.frequency.exponentialRampToValueAtTime(880, now + 0.045);
      gain.gain.setValueAtTime(0.05, now);
      gain.gain.exponentialRampToValueAtTime(0.0001, now + 0.055);
      osc.connect(gain);
      gain.connect(this.masterGain);
      osc.start(now);
      osc.stop(now + 0.06);
    },

    playInspect() {
      if (!this.enabled || !this._ensureContext()) return;
      this._playMarimbaNote(523.25, 0.075);
      window.setTimeout(() => this._playMarimbaNote(659.25, 0.065), 80);
    },

    playApprove() {
      if (!this.enabled || !this._ensureContext()) return;
      this._playMarimbaNote(392.00, 0.075);
      window.setTimeout(() => this._playMarimbaNote(523.25, 0.08), 85);
      window.setTimeout(() => this._playMarimbaNote(659.25, 0.085), 170);
    },

    playStamp() {
      if (!this.enabled || !this._ensureContext()) return;
      const now = this.ctx.currentTime;
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();
      osc.type = 'sine';
      osc.frequency.setValueAtTime(190, now);
      osc.frequency.exponentialRampToValueAtTime(82, now + 0.09);
      gain.gain.setValueAtTime(0.11, now);
      gain.gain.exponentialRampToValueAtTime(0.0001, now + 0.11);
      osc.connect(gain);
      gain.connect(this.masterGain);
      osc.start(now);
      osc.stop(now + 0.12);
    },

    playWater() {
      if (!this.enabled || !this._ensureContext()) return;
      const now = this.ctx.currentTime;
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();
      osc.type = 'sine';
      osc.frequency.setValueAtTime(420, now);
      osc.frequency.exponentialRampToValueAtTime(760, now + 0.09);
      gain.gain.setValueAtTime(0.06, now);
      gain.gain.exponentialRampToValueAtTime(0.0001, now + 0.11);
      osc.connect(gain);
      gain.connect(this.masterGain);
      osc.start(now);
      osc.stop(now + 0.12);
    },

    playHarvest() {
      if (!this.enabled || !this._ensureContext()) return;
      this._playMarimbaNote(440.00, 0.075);
      window.setTimeout(() => this._playMarimbaNote(587.33, 0.08), 70);
    },

    playCook() {
      if (!this.enabled || !this._ensureContext()) return;
      this._playMarimbaNote(329.63, 0.07);
      window.setTimeout(() => this._playMarimbaNote(440.00, 0.075), 95);
    },

    playWorkTap() {
      this.playStamp();
    }
  };

  window.OasisAudio = OasisAudio;
  window.toggleOasisAudio = function () {
    return OasisAudio.toggle();
  };

  document.addEventListener('DOMContentLoaded', () => {
    OasisAudio.syncToggleButtons();
  });
})();
