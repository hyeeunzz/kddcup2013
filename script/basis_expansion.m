function [ Y ] = basis_expansion( X )
%BASIS_EXPANSION Summary of this function goes here
%   Detailed explanation goes here

X2 = [ones(size(X,1), 1) X];
d = size(X2, 2);
Y = [];
for i=1:d
    for j=i:d
        Y = [Y X2(:,i).*X2(:,j)];
    end
end

end

