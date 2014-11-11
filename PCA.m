data = dlmread('bool_data.txt');
dims = 101
data = reshape(data,[],dims);
rp = randperm(length(data));
data=data(rp,:);

[coeff, score, latent] = princomp(data(:, 1:dims-1));

P = coeff(:, 1:52);

pca_data = transpose(transpose(P) * transpose(data(:, 1:dims-1)));

pca_data = horzcat(pca_data, data(:, dims));
train_data = pca_data(1:length(pca_data)/2,:);
test_data = pca_data(length(pca_data)/2+1:end,:);
active_feat = [1:52];

label = 53;
mean1 = mean(train_data(train_data(:,label)==0,active_feat));
mean2 = mean(train_data(train_data(:,label)==1,active_feat));

covm1 = cov(train_data(train_data(:,label)==0,active_feat));
covm2 = cov(train_data(train_data(:,label)==1,active_feat));
prior1tmp = length(train_data(train_data(:,label)==0)); 
prior2tmp = length(train_data(train_data(:,label)==1));
prior1 = prior1tmp/(prior1tmp+prior2tmp);
prior2 = prior2tmp/(prior1tmp+prior2tmp);

correct = 0;
wrong = 0;

for i = 1:length(test_data)
    lklhood1 = exp(((test_data(i,active_feat)-mean1) * inv(covm1) * transpose((test_data(i,active_feat)-mean1))) / (-2)) / sqrt(det(covm1));
    lklhood2 = exp(((test_data(i,active_feat)-mean2) * inv(covm2) * transpose((test_data(i,active_feat)-mean2))) / (-2)) / sqrt(det(covm2));

    post1 = lklhood1 * prior1;
    post2 = lklhood2 * prior2;

    if(post1 > post2 && test_data(i,label) == 0)
        correct = correct+1;
    elseif(post1 < post2 && test_data(i,label) == 1)
        correct = correct+1;
    else
        wrong = wrong+1;
    end
end

accuracy = correct / (correct + wrong);

