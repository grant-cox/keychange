clear

[unshifted,fs] = audioread('sine.wav');
[shifted,fs2] = audioread('sineShift.wav');

unshiftedFFT = fft(unshifted);
shiftedFFT = fft(shifted);

bins = 0:size(shifted)-1;
bins(:,1) = fs * 2 / bins(:,1);

figure('Name','FFT original vs. shifted','NumberTitle','off');
unshiftedMag = abs(unshiftedFFT);
shiftedMag = abs(shiftedFFT);
plot(bins,unshiftedMag,'g',bins,shiftedMag,'b');
legend('y = unshiftedMag','y = shiftedMag');
xlabel('bin');
ylabel('magnitude(FFT(signal))');
title('Fleetwood Mac Shifted Down 1 Semitone');
grid;

bsize = size(bins);
xl = bsize(2) / 2;
xlim([0 xl]);