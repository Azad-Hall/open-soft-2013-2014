function W = haarWavelet(image,level)
    
    %dimensions of image
    [h,w,d] = size(image);
    
    %allocate W
    W = zeros(h,w,d);
    for r = 1:h
        for c = 1:w
            W(r,c,1) = image(r,c,1);
            W(r,c,2) = image(r,c,2);
            W(r,c,3) = image(r,c,3);
        end
    end
    
    h = h/2^level;
    w = w/2^level;
    
    %allocate intermediate matrix
    a = zeros(h*w/4, 4);

    for color = 1:d
        for r = 1:2:h
            for c = 1:2:w
                for i = 0:1
                    for j = 0:1
                        a(floor(r/2)*floor(w/2) + floor(c/2) + 1, i*2 + j + 1) = image(r+i, c+j,color);
                    end
                end
            end
        end

        %perform haar transform
        a = [a(:,1)+a(:,2)+a(:,3)+a(:,4) a(:,1)-a(:,2)+a(:,3)-a(:,4) a(:,1)+a(:,2)-a(:,3)-a(:,4) a(:,1)-a(:,2)-a(:,3)+a(:,4)];
        a = floor(a/4);

        %convert to wavelet
        %LL
        for r = 1:h/2
            W(r,1:w/2,color) = a(((r-1)*w/2 + 1):(r*w/2), 1);            
        end
        %LH
        for r = 1:h/2
            W(r,(w/2+1):w,color) = a(((r-1)*w/2 + 1):(r*w/2), 2);            
        end
        %HL
        for r = 1:h/2
            W(r+h/2,1:w/2,color) = a(((r-1)*w/2 + 1):(r*w/2), 3);            
        end
        %HH
        for r = 1:h/2
            W(r+h/2,(w/2+1):w,color) = a(((r-1)*w/2 + 1):(r*w/2), 4);            
        end
    end
end

