data = csvread ('result23_20_20161104.csv');

sz = size (data);
idx = 1;
lc_16 = 5.0 / 65535;
lc_10 = 1.1 / 1024;
while idx <= sz(1)
    tmp = data (idx, 2) * lc_16 * 10;
    i_lower  = find(table <= tmp, 1, 'last');
    i_higher = find(table >= tmp, 1, 'first');

    temp(idx) = ((i_lower - 1) + (i_higher - 1)) / 2 + 22;
    thermoelectric_v (idx) = data (idx, 4) * lc_10 *1000;
    idx = idx + 1;
end