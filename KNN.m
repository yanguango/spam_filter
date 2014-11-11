data = dlmread('data.txt');
data = reshape(data,[],101);
rp = randperm(length(data));
data=data(rp,:);

train_data = data(1:length(data)/2,:);
test_data = data(length(data)/2+1:end,:);
active_feat = [1:100];
label = 101;
correct = 0;
wrong = 0;

for i = 1:length(test_data)
    sample = test_data(i, active_feat);
    dist = train_data(:, active_feat) - repmat(sample, length(train_data), 1);
    dist = dist * dist';
    fin_dist = diag(dist);
    [min_d index] = min(fin_dist);
    
    if(test_data(i,label) == train_data(index,label))
        correct = correct+1;
    else
        wrong = wrong+1;
    end
end

accuracy = correct / (correct + wrong);
