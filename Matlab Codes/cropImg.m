%Created by Tarmizi Adam, 10/3/ 2015
%The purpose of this snippet is to read Images and their corresponding
% xml annotation files for cropping the images according to their bounding
% box in the xml files. It is used to crop human/person in a given image.
%The specific images to crop for their bounding box is the willow actions
%database image. It can be found at http://www.di.ens.fr/willow/research/stillactions/

% Input: Image(s), corresponding xml file (bounding box data file)
% Output: Cropped image(s) according to bounding box

%The cropped image will be saved in another folder. The corpped image will
%be a smaller image with just the person.

%Dependencies: This uses the "xml_read.m" for parsing the xml files. 
%the "xml_read.m" is written by Jarek Tuszynski, SAIC, jaroslaw.w.tuszynski_at_saic.com


%Read the image files
[fileName,pathName] = uigetfile('.jpg','Select images', 'MultiSelect','on');
fileName = fullfile(pathName, fileName);

%Read corresponding xml annotation files
[xmlFileName,xmlPathName] = uigetfile('.xml','Select xml file', 'MultiSelect','on');
xmlFileName = fullfile(xmlPathName, xmlFileName);

%xml() = zeros(1,length(fileName)); %Pre allocate array of struct;
curr = [pwd '\WillowCropped'];

for k = 1:length(fileName) 
  
    oriImg = imread(fileName{1,k});
    oriImg = rgb2gray(oriImg);
    
    xml(k) = xml_read(xmlFileName{1,k});
    
    %If there is many person doing an action execute this
    if length(xml(1,k).object) > 1 
        for i = 1:length(xml(1,k).object)
          
          if isrow(xml(1,k).object) == 0
              xml(1,k).object = xml(1,k).object';
          end
          xMin = xml(1,k).object(1,i).bndbox.xmin;
          xMax = xml(1,k).object(1,i).bndbox.xmax;
          
          yMin = xml(1,k).object(1,i).bndbox.ymin;
          yMax = xml(1,k).object(1,i).bndbox.ymax;
          
          width = xMax - xMin;   %Width of the cropped image
          height = yMax - yMin;  % Height of the cropped image
          
          cropd = imcrop(oriImg, [xMin yMin width height]);

          cropd = imresize(cropd,[300 200]);
          imwrite(cropd, [curr '\many\' num2str(i) xml(1,k).filename]); 
        end       
    %Else if there is only one human (i.e one bounding box) do this    
    else
        %Get the bounding box of the person from the image
        xMin = xml(1,k).object.bndbox.xmin;
        xMax = xml(1,k).object.bndbox.xmax;

        yMin = xml(1,k).object.bndbox.ymin;
        yMax = xml(1,k).object.bndbox.ymax;

        width = xMax - xMin;   %Width of the cropped image
        height = yMax - yMin;  % Height of the cropped image

        cropd = imcrop(oriImg, [xMin yMin width height]);
        
        cropd = imresize(cropd,[300 200]);
        %Save the cropped image
        imwrite(cropd, [curr '\' xml(1,k).filename]); 
        
    end
    
end

