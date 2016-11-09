figure;
x = linspace (0,3781.9, 89811);
%x = [0:3781.9:89811];
y_fit = 224 * exp (-0.0006164*x) + 27;
subplot(2,1,1);
plot (t(22:length(t)), y(22:length(y),1), 'g'); hold on;
plot (x,y_fit, 'red');
hold off;
subplot(2,1,2);
plot (y(22:length(y)), thermoelectric_v(22:length(thermoelectric_v)), '*')
grid on
