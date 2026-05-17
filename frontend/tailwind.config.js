/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ['./src/**/*.{js,jsx,ts,tsx}', './public/index.html'],
  theme: {
    extend: {
      fontFamily: {
        heading: ['"Cinzel Decorative"', 'serif'],
        body: ['"EB Garamond"', 'serif'],
        mono: ['"JetBrains Mono"', 'monospace'],
      },
      colors: {
        void: '#050505',
        panel: '#101014',
        parchment: '#2C2621',
        stone: '#3A3632',
        gold: '#A5854C',
        goldlight: '#D4AF6A',
        crimson: '#E11D48',
        rarity: {
          common: '#B0B0B0',
          magic: '#3B82F6',
          rare: '#FBBF24',
          epic: '#A855F7',
          legendary: '#F97316',
          mythic: '#E11D48',
        },
      },
      animation: {
        'pulse-glow': 'pulseGlow 2s ease-in-out infinite',
        'float-up': 'floatUp 0.8s cubic-bezier(0.17,0.67,0.32,1.27) forwards',
        'shake': 'shake 0.35s ease-in-out',
        'ember': 'ember 6s linear infinite',
        'fade-in-up': 'fadeInUp 0.4s ease-out forwards',
      },
      keyframes: {
        pulseGlow: {
          '0%,100%': { boxShadow: '0 0 8px rgba(165,133,76,0.3)' },
          '50%': { boxShadow: '0 0 24px rgba(165,133,76,0.7)' },
        },
        floatUp: {
          '0%': { transform: 'translate(-50%,0) scale(0.8)', opacity: 0 },
          '15%': { transform: 'translate(-50%,-10px) scale(1.2)', opacity: 1 },
          '100%': { transform: 'translate(-50%,-60px) scale(0.9)', opacity: 0 },
        },
        shake: {
          '0%,100%': { transform: 'translate(0,0)' },
          '20%': { transform: 'translate(-4px,2px)' },
          '40%': { transform: 'translate(3px,-3px)' },
          '60%': { transform: 'translate(-2px,4px)' },
          '80%': { transform: 'translate(4px,1px)' },
        },
        ember: {
          '0%': { transform: 'translateY(0) translateX(0)', opacity: 0 },
          '10%': { opacity: 0.8 },
          '100%': { transform: 'translateY(-200px) translateX(20px)', opacity: 0 },
        },
        fadeInUp: {
          '0%': { transform: 'translateY(10px)', opacity: 0 },
          '100%': { transform: 'translateY(0)', opacity: 1 },
        },
      },
    },
  },
  plugins: [],
};
