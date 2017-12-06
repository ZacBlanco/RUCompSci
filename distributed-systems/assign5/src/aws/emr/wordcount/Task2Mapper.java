package aws.emr.wordcount;

import java.io.IOException;
import java.util.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;

public class Task2Mapper extends MapReduceBase implements
		Mapper<LongWritable, Text, Text, IntWritable> {
	private final static IntWritable one = new IntWritable(1);
	private Text word = new Text();

	public void map(LongWritable key, Text value,
			OutputCollector<Text, IntWritable> output, Reporter reporter)
			throws IOException {
		String line = value.toString();
		StringTokenizer tokenizer = new StringTokenizer(line);
		while (tokenizer.hasMoreTokens()) {
			 
			/*
 			************* Task 2 **********
			*/

		    word.set(tokenizer.nextToken());
			String text = word.toString();
			
			if (text.matches("\\b\\d{2}:\\d{2}:\\d{2}\\b")) {
				output.collect(word, one);
			}
 
			
		}
	}
}