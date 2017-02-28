% Display a file
I = imread('Satellite.png');
imshow(I);

zoom on; 
waitfor(gcf, 'CurrentCharacter', char(13))
zoom reset
zoom off

[c,r] = ginput;
A = horzcat(c,r)

for i = 1:size(A,1)
    fprintf(fileID,'%f, ',A(i,:));
    fprintf(fileID,'\n');
end
