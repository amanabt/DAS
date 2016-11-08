rng  default;
Fs = 23.7531;
t = (0:L)*T;
fc = 0.000001;
wn = (2 / Fs) * fc;
b = fir1 (20, wn, 'low', kaiser (21,3));
y = filter (b, 1, temp);
plot (t, temp); hold on;
plot (t, y);