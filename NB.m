data = dlmread('tf_data.txt');
dims = 201
data = reshape(data,[],dims);
rp = randperm(length(data));
data=data(rp,:);

[coeff, score, latent] = princomp(data(:, 1:dims-1));
pc_dims = 120;
P = coeff(:, 1:pc_dims);

pca_data = transpose(transpose(P) * transpose(data(:, 1:dims-1)));

pca_data = horzcat(pca_data, data(:, dims));
train_data = pca_data(1:length(pca_data)/2,:);
test_data = pca_data(length(pca_data)/2+1:end,:);
active_feat = [1:pc_dims];

label = pc_dims + 1;

prior1tmp = length(train_data(train_data(:,label)==0)); 
prior2tmp = length(train_data(train_data(:,label)==1));
prior1 = prior1tmp/(prior1tmp+prior2tmp);
prior2 = prior2tmp/(prior1tmp+prior2tmp);

mean1 = [];
var1 = [];
mean2 = [];
var1 = [];

for i = 1:pc_dims - 1
    m = mean(train_data(train_data(:,label)==0, i));
    mean1(i) = m;
    v = var(train_data(train_data(:,label)==0, i));
    var1(i) = v;
    
    m = mean(train_data(train_data(:,label)==1, i));
    mean2(i) = m;
    v = var(train_data(train_data(:,label)==1, i));
    var2(i) = v;
end

correct = 0;
wrong = 0;
for i = 1:length(test_data)
    lklhood1 = 1;
    lklhood2 = 1;
    
    for j = 1:length(mean1)
        lklhood1 = lklhood1 * normpdf(test_data(i, j), mean1(j), var1(j));
        lklhood2 = lklhood2 * normpdf(test_data(i, j), mean2(j), var2(j));
    end
    
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

accuracy = correct / (correct + wrong)


