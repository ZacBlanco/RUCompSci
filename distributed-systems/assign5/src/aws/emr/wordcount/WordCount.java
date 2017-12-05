package aws.emr.wordcount;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;

public class WordCount {

	public static void main(String[] args) throws Exception {
		
		JobConf conf = new JobConf(WordCount.class);
		//conf.setJobName("WordCount"); // Task 1
		//conf.setJobName("TimeCount"); // Task 2
		//conf.setJobName("RequestCount"); // Task 3
		conf.setJobName("IPCount"); // Task 4

		conf.setOutputKeyClass(Text.class);
		conf.setOutputValueClass(IntWritable.class);

		conf.setMapperClass(WordCountMapper.class);
		conf.setCombinerClass(WordCountReducer.class);
		conf.setReducerClass(WordCountReducer.class);

		conf.setInputFormat(TextInputFormat.class);
		conf.setOutputFormat(TextOutputFormat.class);

		FileInputFormat.setInputPaths(conf, new Path(args[0]));
		FileOutputFormat.setOutputPath(conf, new Path(args[1]));

		JobClient.runJob(conf);
	}
}