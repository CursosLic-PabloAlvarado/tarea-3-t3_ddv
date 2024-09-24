pkg load signal
function plot_filter(filename,fs=48000)
    % Load the filter
    Data = load(filename, 'sos');
    sos = Data.sos;
    [b,a] = sos2tf(sos);

    % logartimic sacle frequency
    log_freq = logspace(0, log10(fs/2), 1000);
    omega = 2*pi*log_freq/fs;

    % frequency response
    H = polyval(b,exp(j*omega))./polyval(a,exp(j*omega));

    % magnitude and phase
    magntiude = abs(H);

    %convert to degrees
    phase = angle(H)*180/pi;
    %phase = mod(phase+180, 360) - 360;

    figure;

    subplot(2,1,1);
    semilogx(log_freq, 20*log10(magntiude), 'LineWidth', 2);
    axis([1 fs/2 -100 10]);
    xlabel('F [Hz]');
    ylabel('|H(F)| [dB]');
    grid on;

    subplot(2,1,2);
    semilogx(log_freq, phase, 'LineWidth', 2);
    %axis([1 fs/2 -360 0]);
    xlabel('F [Hz]');
    ylabel('\angle H(F) [°]');	
    grid on;

    %figure;
    %zplane (b,a);
    %title ("Zero pole form");

endfunction
