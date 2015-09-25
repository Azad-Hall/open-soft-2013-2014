function features = getFeatures(image)
    grayimg = double(rgb2gray(image));
    [h,w,d] = size(image);
    W = haarWavelet(image,0);
    vars = zeros(512/8-1,512/8-1);
    for row = 1:8:504,
        for col = 1:8:504,
            mat = grayimg(row:row+8,col:col+8);
            vec = mat(:);
            vars((row-1)/8+1,(col-1)/8+1) = var(vec);            
        end
    end
    
    lh = W(1:256,257:512);
    hl = W(1:256,257:512);
    hh = W(1:256,257:512);
    lhenergy = zeros(512/8-1,512/8-1);
    hlenergy = zeros(512/8-1,512/8-1);
    hhenergy = zeros(512/8-1,512/8-1);
    
    for row = 1:4:252,
        for col = 1:4:252,
            
            mat = lh(row:row+4,col:col+4);
            vec = mat(:);
            lhenergy((row-1)/4+1,(col-1)/4+1) = var(vec)+(mean(vec))^2;            
            
            mat = hl(row:row+4,col:col+4);
            vec = mat(:);
            hlenergy((row-1)/4+1,(col-1)/4+1) = var(vec)+(mean(vec))^2;
            
            mat = hh(row:row+4,col:col+4);
            vec = mat(:);
            hhenergy((row-1)/4+1,(col-1)/4+1) = var(vec)+(mean(vec))^2;
        end
    end
    
    features = [vars(:),lhenergy(:),hlenergy(:),hhenergy(:)];
