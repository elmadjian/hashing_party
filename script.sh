#!/bin/bash

TEXTO=$1
echo `java -cp stanford-corenlp-1.3.5.jar:stanford-corenlp-1.3.5-models.jar:xom.jar:joda-time.jar:jollyday.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma -file $TEXTO -outputFormat text`
