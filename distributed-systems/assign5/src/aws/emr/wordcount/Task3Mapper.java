package aws.emr.wordcount;

import java.io.IOException;
import java.util.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;

public class Task3Mapper extends MapReduceBase implements
		Mapper<LongWritable, Text, Text, IntWritable> {
	private final static IntWritable one = new IntWritable(1);
	private Text word = new Text();

	public void map(LongWritable key, Text value,
			OutputCollector<Text, IntWritable> output, Reporter reporter)
			throws IOException {
		String line = value.toString();
		StringTokenizer tokenizer = new StringTokenizer(line);
		while (tokenizer.hasMoreTokens()) {
			
			word.set(tokenizer.nextToken());
			String text = word.toString();
			
			/*
			************* Task 3 **********
			*/
			word.set(tokenizer.nextToken());
			String text = word.toString();
			
			if (text.matches("\\b\\d{2}:\\d{2}:\\d{2}\\b")) {
				text = text.substring(0, 2);
				word.set(text);
				output.collect(word, one);
			}
			
			 
			
		}
	}
}