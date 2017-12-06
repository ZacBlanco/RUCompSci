package aws.emr.wordcount;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;

public class WordCount {

	public static void main(String[] args) throws Exception {
		
		// UNCOMMENT/Recomment to compile different jobs.		
//
//		JobConf wordcount = setupConf("WordCount", Task1Mapper.class, WordCountReducer.class, args);
//		JobClient.runJob(wordcount);

		// JobConf timecount = setupConf("TimeCount", Task2Mapper.class, WordCountReducer.class, args);
		// JobClient.runJob(timecount);

		 JobConf reqcount = setupConf("RequestCount", Task3Mapper.class, WordCountReducer.class, args);
		 JobClient.runJob(reqcount);

		// JobConf ipcount = setupConf("IPCount", Task4Mapper.class, WordCountReducer.class, args);
		// JobClient.runJob(ipcount);

	}

	public static JobConf setupConf(String jobname, Class mapper, Class reducer, String[] args) {
		
		JobConf conf = new JobConf(WordCount.class);
		conf.setJobName(jobname);

		conf.setOutputKeyClass(Text.class);
		conf.setOutputValueClass(IntWritable.class);

		conf.setMapperClass(mapper);
		conf.setCombinerClass(reducer);
		conf.setReducerClass(reducer);

		conf.setInputFormat(TextInputFormat.class);
		conf.setOutputFormat(TextOutputFormat.class);
		
		FileInputFormat.setInputPaths(conf, new Path(args[0]));
		FileOutputFormat.setOutputPath(conf, new Path(args[1]));
		
		return conf;
	}
}