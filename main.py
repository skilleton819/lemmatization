import numpy as np
import time
startstart = time.clock()
from nltk.stem import WordNetLemmatizer
import nltk
lemmatizer_output = WordNetLemmatizer()

string = ''' '''
splitString = nltk.word_tokenize(string)
trial = []

trial = nltk.pos_tag(splitString)
trial = np.array(trial)
trial.flatten()

for k in range(len(trial)):
    if (trial[k][1] == 'NNS') or (trial[k][1] == 'NN') or (trial[k][1] == 'VBZ'):
        trial[k][0] = lemmatizer_output.lemmatize(trial[k][0])
my_lst_str = ' '.join(map(str, trial[:,0]))

print ("Total time:",str((time.clock()-startstart)),"secs")