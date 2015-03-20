%Created by Tarmizi Adam, 17/3/ 2015
%The purpose of this snippet is to resize Images
% Input: Image(s)
% Output: resized image(s) 

%The resized image will be saved in the current image folder user selected. 


%Read the image files
[fileName,pathName] = uigetfile('.jpg','Select images', 'MultiSelect','on');
fileName = fullfile(pathName, fileName);

curr = pwd;

for k = 1:length(fileName) 
  
    oriImg = imread(fileName{1,k});
    
    if size(oriImg,2) == 3  % if RGB image, convert to grayscale.
        oriImg = rgb2gray(oriImg);   
    end
    
    resizedImg = imresize(oriImg,[200 300]);
    imwrite(resizedImg, fileName{1,k});   % save resized image to current folder
end

