clc;
clear;
rng(1);
%--------**************����һЩ��֪����************--------------
sample_num = 20000;                 % ��������
N = 16;                             % �˲�������
muw = 0.0005;                        % ����������
mus_min = 0.0075;                   % ��ģ�˲���������Сֵ
mus_max = 0.025;                   % ��ģ�˲����������ֵ
n = 1:sample_num;                   % ��ɢʱ��
fs = 2000;
% ������Ƶ�ź�
S1 = sin(2*pi*50*n/fs);
S2 = sin(2*pi*150*n/fs);
S3 = sin(2*pi*300*n/fs);
signal = S1 + S2 + S3;  % ��Ƶ�ź�

xs = awgn(signal, 30, 'measured');

vs = sqrt(0.05)*randn(1, sample_num);   % ����0.5�ĸ���������
Pw = [0.2 -0.001 0.001 0.4 0.9 -0.1 -0.5 -0.03 0.2 -0.05 0.05 -0.001 0.01 0.3 0.06 -0.5 -0.5 -0.1 0.4 -0.01 0.01 -0.003 0.003 0.4 0.8 -0.2 -0.5 -0.1 0.4 -0.05 0.2 -0.4];
Sw = [0.05 -0.4 0.87 0.1 -0.38 0.01 0.1 0 0 0 0 0 0 0.04 0 0.6];
delta_s = zeros(1, length(xs));
lambda = 0.99;
%--------**************��ʼ��һЩ����************------------------

X = zeros(1, N);                    % forѭ���������ź�
Y = zeros(1, N);                    % �����źž��˲���
V = zeros(1, N);                    % ����������
Cx = zeros(1, N);                   % �˲�-x�ź�ʸ��
w = zeros(1, N);                    % ��·���˲���
Sw_estimated = zeros(1, N);          % ���ƵĴμ�·��
en = zeros(1, length(xs));      	 % ����ź�
fn = zeros(1, length(xs));      	 % ����������ź�
gn = zeros(1, length(xs));
Pe = zeros(1, length(xs));         	 % ����ź�����
Pf = zeros(1, length(xs));         	 % ����������ź�����
Ru = zeros(1, length(xs));      	 % ������������
Rn = zeros(1, length(xs));
dn = filter(Pw, 1, xs);              % �����ź�
ds = zeros(1, length(xs));
norm_w = zeros(1, length(xs));
norm_s = zeros(1, length(xs));
w_val = 0.709*1.5;
s_val = 1.2168*1.5;
a = 1;
%--------**************����·��������Ӧ�˲���ϵ��֮��Ĺ�ϵ************------------------
% ���߽�ģ
for k = 1:sample_num
   
    % �ź�����
    X(2:end) = X(1:end-1);
    X(1) = xs(k);
    
    % �˲�������ź�
    Y(2:end) = Y(1:end-1);
    Y(1) = X*w';

    % ����������
    V(2:end) = V(1:end-1);
    V(1) = a*vs(k);

    % ����������ź�  
    en(k) = dn(k) - (Y-V)*Sw'; 

    % ��ģ�˲�������ź�  
    fn(k) = en(k) - V*Sw_estimated';
    %VSSģ��
    if k==1
        Pe(k) = sum(en.^2);%����ֵ
        Pf(k) = sum(fn.^2);%����ֵ
    end
    if k>1
        Pe(k) = lambda*Pe(k-1) + (1-lambda)* en(k).^2;%���ʴ���ʽ
        Pf(k) = lambda*Pf(k-1) + (1-lambda)* fn(k).^2;
    end
    Ru(k) = Pf(k)/Pe(k);
    mus = mus_min*Ru(k) + (1 - Ru(k))*mus_max;
    a = Ru(k);%���ڰ���������
    % ���÷������ƹ��ƴμ�·��
    Sw_estimated = Sw_estimated + mus*V*fn(k);
    norm_s(k) = norm(Sw_estimated);
    if norm_s(k)>s_val
        Sw_estimated = (s_val/norm_s(k))*Sw_estimated;       
    end
    %����μ�·����ģ���
    sa = sum((Sw_estimated-Sw).^2);
    sb = sum(Sw.^2);
    ds(k) = 10*log10(sa/sb);
    % �˲�-x�ź�ʸ��
    Cx(2:end) = Cx(1:end-1);
    Cx(1) = X*Sw_estimated';
   
    % MFxLMS
    % gn(k) = fn(k) + Y*Sw_estimated' - Cx*w';
    % �������ƿ���������
    w = w + muw*Cx*fn(k); 
    norm_w(k) = norm(w);
    if norm_w(k)>w_val
       w = (w_val/norm_w(k))*w;
    else
       w = w + muw*Cx*gn(k); 
    end
    %���㽵����
    as = sum(en(1:k).^2)/k;
    bs = sum(dn(1:k).^2)/k;
    Rn(k) = -10*log(as/bs);
    %����ÿһ�θ��µķ���ֵ


    %�μ�·�����������仯
    if k == 100000
       Sw = Sw.*randn(1,16);
    end
end
% �����ź�ͼ��
figure(1);
plot(en);
hold on
legend('en');
figure(2);
plot(ds);
legend('ds');
title('�μ�·����ģ���');
hold on
figure(3);
plot(Rn);
legend('Rn');
title('������');
hold on





