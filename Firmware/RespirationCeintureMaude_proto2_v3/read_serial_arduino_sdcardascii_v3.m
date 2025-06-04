%%
% By ACL, 2020-04-23
% v1: Lecture de la premiere acquisition dans un vecteur.
% v2: Ajout de combien de valeur on lit par secteur et combien de pas de
% temps
% v3: Enregistre tous les fichiers dans un CSV
% v4: Enregistre tous les fichiers en csv ou juste quelques uns
% v5: Facteurs de conversion (ex. pour des floats diviser par 10000) et bug
% que on enregistrait que la premiere ligne du secteur. Et ferme lenvoie de
% donnees de debuggage sur cortex pour ecouter les infos correctement.
%%//si bug car trop long, pourrait envoyer des blocs de données que matlab store avant d'écrire et matlab réenvoie un ok avant de continuer.
%%
clear, close all;
clc



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% USER INPUT
sddatasize=10;
floatval = [1 1 1000 1 1 1 1000 1000 1000 1];

recordAllToCSV = 1;% If equal to 1, transfer all acqusitions to CSV
if (recordAllToCSV == 0)%or we can shoose only a few ones to transfer to CSV
    recordAcquisitions = 14; %if you want only given acquisitions
%     recordAcquisitions = [0,2,4]; %if you want only given acquisitions
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%% Ouverture du port série
delete(instrfindall);
s = serial('COM7'); %set communication port 
s.BaudRate = 115200;%115200;% ce pourrait etre bien de le diminuer pour etre certain de ne pas avoir d'erreur
s.InputBufferSize = 2048;%2048 
% set(s,'Terminator','LF') % set(s,'Terminator','CR') % set(s,'Terminator','LF/CR') % set(s,'Terminator','CR/LF') 

fopen(s); %open serial port  
% fprintf(s,'%s','list');fwrite(s,13,'char');% fwrite(s,10,'char');% fprintf(s,'%s\r\n','list');% fprintf(s,'%s\n','list');fwrite(s,13,'char');% fprintf(s,'list');% fwrite(s,13,'char');



%% Fermeture de l'envoie série du Cortex et vider le buffer seriel
pause(1);
fprintf(s,'%s','seriallog off\n');
% flush(s);% pause(1);% fscanf(s);% flush(s,"input");
pause(1);
flushinput(s);
pause(1);
flushinput(s);

%% Determine the number of files
fprintf(s,'%s','listfiles\n');
pause(2);
errorlist=0;
numberofacquisitions = 1;

while(1)
    an= fscanf(s);
    if (strcmp(an(1:end-1),'endserial'))
        break;
    end
    datafileexist=split(an,"Data");
    datafiledat=split(an,".dat");
    if length(datafileexist)==2 && length(datafiledat)==2
        filelist{numberofacquisitions}=an(1:end-2);
        numberofacquisitions = numberofacquisitions + 1;
    end
end 
filelist



%% Read acquisitions
if recordAllToCSV==1
    recordAcquisitions=1:length(filelist);
else
    recordAcquisitions=recordAcquisitions+1;
end


for iterateacquisition_list=1:length(recordAcquisitions)
    iterateacquisition = recordAcquisitions(iterateacquisition_list);
    disp([filelist{iterateacquisition},'   ',num2str(iterateacquisition_list),'/',num2str(length(recordAcquisitions))]);

    OutputFile = [filelist{iterateacquisition}(1:end-4),'_Export.csv'];
    fid = fopen(OutputFile,'w');
    %fprintf(fid,'%s,', col_header{:});
    %fprintf(fid,'\n');
    fprintf(s,'%s\n',strcat('readdata'," ",filelist{iterateacquisition}(5:8)));

    % pause(1);

    readdatacounter=0;
%     nbytes=0;
%         while(1)
%             pause(0.01);
%             an = fscanf(s);
%             readdata=an;
%             readdata(end-1:end)=[];
%             readdatasplit = split(readdata,",");
%             %an
%             %length(readdatasplit) 
%             if (length(readdatasplit)~=sddatasize)
%                disp(['error_sector_',filelist{iterateacquisition}]);
%                readdata
%             end
%             
%         end
    

    while(1)
        an = fscanf(s);

            if (strcmp(an(2:end-1),'endserial'))
                break;
            end

            readdatacounter = readdatacounter+1;

            readdata=an;

            try
                %readdata(end-1:end)=[];
                %if (strcmp(readdata(end),',')) readdata(end)=[]; end
                readdatasplit = split(readdata(2:end-1),",");

                if (length(readdatasplit)~=sddatasize)
                   disp(['error_sector_',filelist{iterateacquisition}]);
                else
                    readdatasplit_floatadjust{sddatasize} = [];
                    for i=1:sddatasize
                        if readdatacounter <= 3
                            readdatasplit_floatadjust{i} =  readdatasplit{i};
                        else
                            readdatasplit_floatadjust{i} =  num2str(str2num(readdatasplit{i})/floatval(i));
                        end
                       %fprintf(fid,'%s,',readdatasplit{i}/floatval(i));
                    end
                    fprintf(fid,'%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n',readdatasplit_floatadjust{1},readdatasplit_floatadjust{2},readdatasplit_floatadjust{3},readdatasplit_floatadjust{4},readdatasplit_floatadjust{5},readdatasplit_floatadjust{6},readdatasplit_floatadjust{7},readdatasplit_floatadjust{8},readdatasplit_floatadjust{9},readdatasplit_floatadjust{10});
                    %fprintf(fid,'\n');
                    %fprintf(fid,'%s\n',readdata./floatval);
                    %fprintf(fid,'%s\n',readdata(2:end-1));                    
                end
            catch
                disp(['error_',filelist{iterateacquisition}]); 
            end

        
    end
    %fprintf('\n');
    fprintf(fid,'\n');
    fclose(fid);
end
    
fprintf(s,'%s\n','seriallog on');

fclose(s);  
   



        
        
      