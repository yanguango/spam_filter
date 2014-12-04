data = dlmread('tf_data.txt');
dims = 201;
data = reshape(data,[],dims);

accuracies = [];

rp = randperm(length(data));
data=data(rp,:);

[coeff, score, latent] = princomp(data(:, 1:dims-1));

max_accuracy = 0;
best_pc_dims = 0;

pc_dims = 120;
P = coeff(:, 1:pc_dims);
dlmwrite('P.txt', transpose(P));
pca_data = transpose(transpose(P) * transpose(data(:, 1:dims-1)));

pca_data = horzcat(pca_data, data(:, dims));
train_data = pca_data(1:length(pca_data)/2,:);
test_data = pca_data(length(pca_data)/2+1:end,:);
active_feat = [1:pc_dims];

label = pc_dims + 1;
mean1 = mean(train_data(train_data(:,label)==0,active_feat));
mean2 = mean(train_data(train_data(:,label)==1,active_feat));
dlmwrite('mean1.txt', mean1);
dlmwrite('mean2.txt', mean2);
covm1 = cov(train_data(train_data(:,label)==0,active_feat));
covm2 = cov(train_data(train_data(:,label)==1,active_feat));
dlmwrite('covm1.txt', covm1);
dlmwrite('covm2.txt', covm2);
prior1tmp = length(train_data(train_data(:,label)==0)); 
prior2tmp = length(train_data(train_data(:,label)==1));
prior1 = prior1tmp/(prior1tmp+prior2tmp);
prior2 = prior2tmp/(prior1tmp+prior2tmp);

correct = 0;
wrong = 0;


for i = 1:length(test_data)
    lklhood1 = mvnpdf(test_data(i,active_feat), mean1, covm1);
    lklhood2 = mvnpdf(test_data(i,active_feat), mean2, covm2);
     disp(lklhood1);
     disp(lklhood2);
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




