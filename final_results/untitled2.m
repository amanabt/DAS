Y = fft(y);

r = 0.01; % range of frequencies we want to preserve

rectangle = zeros(size(Y));
rectangle(1:r+1) = 1;               % preserve low +ve frequencies
rectangle(end-r+1:end) = 1;         % preserve low -ve frequencies
y_rect = ifft(Y.*rectangle);   % full low-pass filtered signal

hold on;
plot(t,y,'g--'); plot(t,x,'k','LineWidth',2); plot(t,y_half,'b','LineWidth',2); plot(t,y_rect,'r','LineWidth',2);
legend('noisy signal','true signal','+ve low-pass','full low-pass','Location','southwest')