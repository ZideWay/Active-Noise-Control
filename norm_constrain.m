clc;
clear;
rng(1);
%--------**************设置一些已知参量************--------------
sample_num = 20000;                 % 采样点数
N = 16;                             % 滤波器长度
muw = 0.0005;                        % 控制器步长
mus_min = 0.0075;                   % 建模滤波器步长最小值
mus_max = 0.025;                   % 建模滤波器步长最大值
n = 1:sample_num;                   % 离散时间
fs = 2000;
% 创建单频信号
S1 = sin(2*pi*50*n/fs);
S2 = sin(2*pi*150*n/fs);
S3 = sin(2*pi*300*n/fs);
signal = S1 + S2 + S3;  % 单频信号

xs = awgn(signal, 30, 'measured');

vs = sqrt(0.05)*randn(1, sample_num);   % 方差0.5的辅助白噪声
Pw = [0.2 -0.001 0.001 0.4 0.9 -0.1 -0.5 -0.03 0.2 -0.05 0.05 -0.001 0.01 0.3 0.06 -0.5 -0.5 -0.1 0.4 -0.01 0.01 -0.003 0.003 0.4 0.8 -0.2 -0.5 -0.1 0.4 -0.05 0.2 -0.4];
Sw = [0.05 -0.4 0.87 0.1 -0.38 0.01 0.1 0 0 0 0 0 0 0.04 0 0.6];
delta_s = zeros(1, length(xs));
lambda = 0.99;
%--------**************初始化一些变量************------------------

X = zeros(1, N);                    % for循环中输入信号
Y = zeros(1, N);                    % 输入信号经滤波器
V = zeros(1, N);                    % 辅助白噪声
Cx = zeros(1, N);                   % 滤波-x信号矢量
w = zeros(1, N);                    % 主路径滤波器
Sw_estimated = zeros(1, N);          % 估计的次级路径
en = zeros(1, length(xs));      	 % 误差信号
fn = zeros(1, length(xs));      	 % 白噪声误差信号
gn = zeros(1, length(xs));
Pe = zeros(1, length(xs));         	 % 误差信号能量
Pf = zeros(1, length(xs));         	 % 白噪声误差信号能量
Ru = zeros(1, length(xs));      	 % 能量调节因子
Rn = zeros(1, length(xs));
dn = filter(Pw, 1, xs);              % 期望信号
ds = zeros(1, length(xs));
norm_w = zeros(1, length(xs));
norm_s = zeros(1, length(xs));
w_val = 0.709*1.5;
s_val = 1.2168*1.5;
a = 1;
%--------**************初级路径和自适应滤波器系数之间的关系************------------------
% 在线建模
for k = 1:sample_num
   
    % 信号输入
    X(2:end) = X(1:end-1);
    X(1) = xs(k);
    
    % 滤波器输出信号
    Y(2:end) = Y(1:end-1);
    Y(1) = X*w';

    % 白噪声输入
    V(2:end) = V(1:end-1);
    V(1) = a*vs(k);

    % 控制器误差信号  
    en(k) = dn(k) - (Y-V)*Sw'; 

    % 建模滤波器误差信号  
    fn(k) = en(k) - V*Sw_estimated';
    %VSS模块
    if k==1
        Pe(k) = sum(en.^2);%赋初值
        Pf(k) = sum(fn.^2);%赋初值
    end
    if k>1
        Pe(k) = lambda*Pe(k-1) + (1-lambda)* en(k).^2;%功率代公式
        Pf(k) = lambda*Pf(k-1) + (1-lambda)* fn(k).^2;
    end
    Ru(k) = Pf(k)/Pe(k);
    mus = mus_min*Ru(k) + (1 - Ru(k))*mus_max;
    a = Ru(k);%调节白噪声因子
    % 采用范数限制估计次级路径
    Sw_estimated = Sw_estimated + mus*V*fn(k);
    norm_s(k) = norm(Sw_estimated);
    if norm_s(k)>s_val
        Sw_estimated = (s_val/norm_s(k))*Sw_estimated;       
    end
    %计算次级路径建模误差
    sa = sum((Sw_estimated-Sw).^2);
    sb = sum(Sw.^2);
    ds(k) = 10*log10(sa/sb);
    % 滤波-x信号矢量
    Cx(2:end) = Cx(1:end-1);
    Cx(1) = X*Sw_estimated';
   
    % MFxLMS
    % gn(k) = fn(k) + Y*Sw_estimated' - Cx*w';
    % 范数限制控制器更新
    w = w + muw*Cx*fn(k); 
    norm_w(k) = norm(w);
    if norm_w(k)>w_val
       w = (w_val/norm_w(k))*w;
    else
       w = w + muw*Cx*gn(k); 
    end
    %计算降噪量
    as = sum(en(1:k).^2)/k;
    bs = sum(dn(1:k).^2)/k;
    Rn(k) = -10*log(as/bs);
    %计算每一次更新的范数值


    %次级路径发生显著变化
    if k == 100000
       Sw = Sw.*randn(1,16);
    end
end
% 绘制信号图形
figure(1);
plot(en);
hold on
legend('en');
figure(2);
plot(ds);
legend('ds');
title('次级路径建模误差');
hold on
figure(3);
plot(Rn);
legend('Rn');
title('降噪量');
hold on





