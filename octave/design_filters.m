% Global parameters for filter
fc = 440;  % cut frequency in Hz
fs = 10000; % sample frequency in Hz
order = 6; % Order of the filter


Wh = fc/(fs/2); % Normalized frequency
[b,a] = butter(order,Wh,'low');
sos = tf2sos(b,a);

save("butter_lowpass.mat",'sos');

Data = load("butter_lowpass.mat", 'sos');
sos = Data.sos;
[b,a] = sos2tf(sos);

% Plot the frequency response
% logartimic sacle frequency
log_freq = logspace(log10(1), log10(fs/2), 1000);
omega = 2*pi*log_freq/fs;
H = polyval(b,exp(j*omega))./polyval(a,exp(j*omega));
magntiude = abs(H);
%grados
phase = angle(H)*180/pi;

figure;

subplot(2,1,1);
semilogx(log_freq, 20*log10(magntiude), 'LineWidth', 2);
axis([1 fs/2 -100 10]);
xlabel('F [Hz]');
ylabel('|H(F)| [dB]');
grid on;

subplot(2,1,2);
semilogx(log_freq, phase, 'LineWidth', 2);
xlabel('F [Hz]');
ylabel('\angle H(F) [°]');	
grid on;




