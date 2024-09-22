pkg load signal
% Global parameters for filter
fc_lowpass = 440;  % cut frequency in Hz
fc_highpass = 600; % cut frequency in Hz
fc_low = 220;  % cut frequency in Hz
fc_high = 1000; % cut frequency in Hz
fs = 48000; % sample frequency in Hz
order = 2; % Order of the filter
rp = 1;     % Ripple in the passband 
rs = 40;    % Attenuation in the stopband 


function save_filter(b, a, filename)
    sos = tf2sos(b,a);
    save(filename, 'sos'); 
endfunction

% ------- Lowpass filters -------
type_filter = "low";
f = 2 * fc_lowpass/fs; % Normalized frequency

% Butterworth filter lowpass
[b,a] = butter(order,f,type_filter);
save_filter(b,a, "butter_lowpass.mat");
%plot_filter("butter_lowpass.mat",fs);

% Elliptic filter lowpass
[b, a] = ellip(order, rp, rs, f, type_filter);
save_filter(b, a, 'ellip_lowpass.mat');
%plot_filter('ellip_lowpass.mat', fs);

% Chebyshev Type I filter lowpass
[b, a] = cheby1(order, rp, f, type_filter);
save_filter(b, a, 'cheby1_lowpass.mat');
%plot_filter('cheby1_lowpass.mat', fs);

% Chebyshev Type II filter  lowpass
[b, a] = cheby2(order, rs, f, type_filter);
save_filter(b, a, 'cheby2_lowpass.mat');
%plot_filter('cheby2_lowpass.mat', fs);

% ------- Highpass filters -------
type_filter = "high";
f = fc_highpass/(fs/2); % Normalized frequency

% Butterworth filter highpass
[b,a] = butter(order,f,type_filter);
save_filter(b,a, "butter_highpass.mat");
%plot_filter("butter_highpass.mat",fs);

% Elliptic filter highpass
[b, a] = ellip(order, rp, rs, f, type_filter);
save_filter(b, a, 'ellip_highpass.mat');
%plot_filter('ellip_highpass.mat', fs);

% Chebyshev Type I filter highpass
[b, a] = cheby1(order, rp, f, type_filter);
save_filter(b, a, 'cheby1_highpass.mat');
%plot_filter('cheby1_highpass.mat', fs);

% Chebyshev Type II filter  highpass
[b, a] = cheby2(order, rs, f, type_filter);
save_filter(b, a, 'cheby2_highpass.mat');
%plot_filter('cheby2_highpass.mat', fs);

% ------- Bandpass filters -------
type_filter = "bandpass";
f = [fc_low fc_high]/(fs/2); % Normalized frequency

% Butterworth filter bandpass
[b,a] = butter(order,f,type_filter);
save_filter(b,a, "butter_bandpass.mat");
%plot_filter("butter_bandpass.mat",fs);

% Elliptic filter bandpass
[b, a] = ellip(order, rp, rs, f);
save_filter(b, a, 'ellip_bandpass.mat');
%plot_filter('ellip_bandpass.mat', fs);

% Chebyshev Type I filter bandpass
[b, a] = cheby1(order, rp, f);
save_filter(b, a, 'cheby1_bandpass.mat');
%plot_filter('cheby1_bandpass.mat', fs);

% Chebyshev Type II filter  bandpass
[b, a] = cheby2(order, rs, f);
save_filter(b, a, 'cheby2_bandpass.mat');
plot_filter('cheby2_bandpass.mat', fs);

% ------- Bandstop filters -------
type_filter = "stop";
f = [fc_low fc_high]/(fs/2); % Normalized frequency

% Butterworth filter bandstop
[b,a] = butter(order,f,type_filter);
save_filter(b,a, "butter_bandstop.mat");
%plot_filter("butter_bandstop.mat",fs);

% Elliptic filter bandstop
[b, a] = ellip(order, rp, rs, f, type_filter);
save_filter(b, a, 'ellip_bandstop.mat');
%plot_filter('ellip_bandstop.mat', fs);

% Chebyshev Type I filter bandstop
[b, a] = cheby1(order, rp, f, type_filter);
save_filter(b, a, 'cheby1_bandstop.mat');
%plot_filter('cheby1_bandstop.mat', fs);

% Chebyshev Type II filter  bandstop
[b, a] = cheby2(order, rs, f, type_filter);
save_filter(b, a, 'cheby2_bandstop.mat');
plot_filter('cheby2_bandstop.mat', fs);



