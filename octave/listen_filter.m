pkg load signal
function listen_filter(filter_file, wav_file, fs=48000)

    Data = load(filter_file, 'sos');
    sos = Data.sos;
    [b,a] = sos2tf(sos);

    [y, fs_audio] = audioread(wav_file);

    if fs_audio ~= fs
        y = resample(y, fs, fs_audio);
        fs_audio = fs;
    end

    y_filtered = filter(b, a, y);

    sound(y_filtered, fs_audio);
end 
