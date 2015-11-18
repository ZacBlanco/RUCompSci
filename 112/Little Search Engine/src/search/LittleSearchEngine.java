package search;

import java.io.*;
import java.util.*;

/**
 * This class encapsulates an occurrence of a keyword in a document. It stores
 * the document name, and the frequency of occurrence in that document.
 * Occurrences are associated with keywords in an index hash table.
 * 
 * @author Sesh Venugopal
 * 
 */
class Occurrence {
	/**
	 * Document in which a keyword occurs.
	 */
	String document;

	/**
	 * The frequency (number of times) the keyword occurs in the above document.
	 */
	int frequency;

	/**
	 * Initializes this occurrence with the given document,frequency pair.
	 * 
	 * @param doc
	 *            Document name
	 * @param freq
	 *            Frequency
	 */
	public Occurrence(String doc, int freq) {
		document = doc;
		frequency = freq;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return "(" + document + "," + frequency + ")";
	}
}

/**
 * This class builds an index of keywords. Each keyword maps to a set of
 * documents in which it occurs, with frequency of occurrence in each document.
 * Once the index is built, the documents can searched on for keywords.
 *
 */
public class LittleSearchEngine {

	/**
	 * This is a hash table of all keywords. The key is the actual keyword, and
	 * the associated value is an array list of all occurrences of the keyword
	 * in documents. The array list is maintained in descending order of
	 * occurrence frequencies.
	 */
	HashMap<String, ArrayList<Occurrence>> keywordsIndex;

	/**
	 * The hash table of all noise words - mapping is from word to itself.
	 */
	HashMap<String, String> noiseWords;

	/**
	 * Creates the keyWordsIndex and noiseWords hash tables.
	 */
	public LittleSearchEngine() {
		keywordsIndex = new HashMap<String, ArrayList<Occurrence>>(1000, 2.0f);
		noiseWords = new HashMap<String, String>(100, 2.0f);
	}

	/**
	 * This method indexes all keywords found in all the input documents. When
	 * this method is done, the keywordsIndex hash table will be filled with all
	 * keywords, each of which is associated with an array list of Occurrence
	 * objects, arranged in decreasing frequencies of occurrence.
	 * 
	 * @param docsFile
	 *            Name of file that has a list of all the document file names,
	 *            one name per line
	 * @param noiseWordsFile
	 *            Name of file that has a list of noise words, one noise word
	 *            per line
	 * @throws FileNotFoundException
	 *             If there is a problem locating any of the input files on disk
	 */
	public void makeIndex(String docsFile, String noiseWordsFile) throws FileNotFoundException {
		// load noise words to hash table
		Scanner sc = new Scanner(new File(noiseWordsFile));
		while (sc.hasNext()) {
			String word = sc.next();
			noiseWords.put(word, word);
		}

		// index all keywords
		sc = new Scanner(new File(docsFile));
		while (sc.hasNext()) {
			String docFile = sc.next();
			HashMap<String, Occurrence> kws = loadKeyWords(docFile);
			mergeKeyWords(kws);
		}

	}

	/**
	 * Scans a document, and loads all keywords found into a hash table of
	 * keyword occurrences in the document. Uses the getKeyWord method to
	 * separate keywords from other words.
	 * 
	 * @param docFile
	 *            Name of the document file to be scanned and loaded
	 * @return Hash table of keywords in the given document, each associated
	 *         with an Occurrence object
	 * @throws FileNotFoundException
	 *             If the document file is not found on disk
	 */
	public HashMap<String, Occurrence> loadKeyWords(String docFile) throws FileNotFoundException {
		// COMPLETE THIS METHOD

		// Define our hashmap to store + values
		HashMap<String, Occurrence> keywords = new HashMap<String, Occurrence>(75, 2.0f);
//		File f = new File(docFile);
//		System.out.println(f.getAbsolutePath());
		Scanner sc = new Scanner(new File(docFile));

		// Scan while we have lines to scan
		while (sc.hasNextLine()) {

			// Let's retrieve the line and split the line by spaces.
			String line = sc.nextLine();
			String[] words = line.split(" ");

			// Now that we have an array of the words on this line, we can retrieve
			// each keyword one by one. Make sure it hasn't returned null
			// Then we can check if it's already occurred.
			// If it has, add one to the occurrence object for it.
			// else create a new occurrence object
			for (String w : words) {
				String word = getKeyWord(w);
				if (word != null) {
					if (keywords.containsKey(word)) {
						keywords.get(word).frequency++;
					} else {
						keywords.put(word, new Occurrence(docFile, 1));
					}
				}
			}

		}

		sc.close(); // Don't want any leakage
		return keywords;
	}

	/**
	 * Merges the keywords for a single document into the master keywordsIndex
	 * hash table. For each keyword, its Occurrence in the current document must
	 * be inserted in the correct place (according to descending order of
	 * frequency) in the same keyword's Occurrence list in the master hash
	 * table. This is done by calling the insertLastOccurrence method.
	 * 
	 * @param kws
	 *            Keywords hash table for a document
	 */
	public void mergeKeyWords(HashMap<String, Occurrence> kws) {

		// We need to add the occurrence for a word to the master list
		// So for every word this should be done
		// First we retrieve the arraylist from the hashmap.
		// If one doesn't exist we should add it to the HashMap
		// Then afterwhich we call
		for (String kw : kws.keySet()) {
			ArrayList<Occurrence> occs = keywordsIndex.get(kw);
			if (occs == null) {
				occs = new ArrayList<Occurrence>();
				occs.add(kws.get(kw));
				keywordsIndex.put(kw, occs);
			} else {
				occs.add(kws.get(kw));
			}
//====================================================================================		
//			TEST CODE
//====================================================================================
//			if(kw.equals("tooth") || kws.equals("vampire")) {
//				System.out.println("Found");
//			}
//====================================================================================		
//			TEST CODE
//====================================================================================
			insertLastOccurrence(occs);
		}
	}

	/**
	 * Given a word, returns it as a keyword if it passes the keyword test,
	 * otherwise returns null. A keyword is any word that, after being stripped
	 * of any TRAILING punctuation, consists only of alphabetic letters, and is
	 * not a noise word. All words are treated in a case-INsensitive manner.
	 * 
	 * Punctuation characters are the following: '.', ',', '?', ':', ';' and '!'
	 * 
	 * @param word
	 *            Candidate word
	 * @return Keyword (word without trailing punctuation, LOWER CASE)
	 */
	public String getKeyWord(String word) {
		// COMPLETE THIS METHOD
		// Let's make everything now lowercase and remove and whitespace.
		String refined = word.toLowerCase().trim();

		// Using regex we're going to replace all the "bad" characters that
		// might TRAIL the word
		refined = refined.replaceAll("[\\.\\,\\!\\?\\;\\:]*$", "");

		// We check to make sure the word matches all lowercase alphabetic chars
		// return word for match, false otherwise
		boolean isNoiseword = (noiseWords.get(refined) != null);
		if (refined.matches("^[a-z]*") && refined.length() > 0 && !isNoiseword) {
			return refined;
		} else {
			return null;
		}
	}

	/**
	 * Inserts the last occurrence in the parameter list in the correct position
	 * in the same list, based on ordering occurrences on descending
	 * frequencies. The elements 0..n-2 in the list are already in the correct
	 * order. Insertion of the last element (the one at index n-1) is done by
	 * first finding the correct spot using binary search, then inserting at
	 * that spot.
	 * 
	 * @param occs
	 *            List of Occurrences
	 * @return Sequence of mid point indexes in the input list checked by the
	 *         binary search process, null if the size of the input list is 1.
	 *         This returned array list is only used to test your code - it is
	 *         not used elsewhere in the program.
	 */
	public ArrayList<Integer> insertLastOccurrence(ArrayList<Occurrence> occs) {
		// COMPLETE THIS METHOD
		ArrayList<Integer> indexes = new ArrayList<Integer>();
				
		
		// In the case that there are less than two occurrences for the given
		// keyword
		if (occs.size() == 1) {
			return null;
		}

		Occurrence lastItem = occs.remove(occs.size() - 1);
		int item = lastItem.frequency;
		int lo = 0, hi = occs.size() - 1; // -1 for last index and -1 for sorted
											// part of the array;
		int mid = 0;
		while (lo <= hi) {
			mid = (hi + lo) / 2;
			indexes.add(mid);
			if (item == occs.get(mid).frequency) {
				break;
			} else if (item < occs.get(mid).frequency) {
				// Here this means that hi should be the same, lo should be mid
				// Because we're sorted in descending order.
				lo = mid + 1;
			} else {
				// Then at this place we have that the hi should be mid
				hi = mid - 1;
			}

		}

		// Finished binary search, so lets add it into the array
		// Questioning whether mid is correct index or if it should be mid+1 or
		// /lo/hi
		if (item >= occs.get(mid).frequency) {
			occs.add(mid, lastItem);
		} else {
			occs.add(mid+1, lastItem);
		}
		return indexes;

	}

	/**
	 * Search result for "kw1 or kw2". A document is in the result set if kw1 or
	 * kw2 occurs in that document. Result set is arranged in descending order
	 * of occurrence frequencies. (Note that a matching document will only
	 * appear once in the result.) Ties in frequency values are broken in favor
	 * of the first keyword. (That is, if kw1 is in doc1 with frequency f1, and
	 * kw2 is in doc2 also with the same frequency f1, then doc1 will appear
	 * before doc2 in the result. The result set is limited to 5 entries. If
	 * there are no matching documents, the result is null.
	 * 
	 * @param kw1
	 *            First keyword
	 * @param kw1
	 *            Second keyword
	 * @return List of NAMES of documents in which either kw1 or kw2 occurs,
	 *         arranged in descending order of frequencies. The result size is
	 *         limited to 5 documents. If there are no matching documents, the
	 *         result is null.
	 */
	public ArrayList<String> top5search(String kw1, String kw2) {
		// COMPLETE THIS METHOD

		// Both ArrayLists should be sorted in descending order
		// This means in worst case we need to merge the first 5 of each list
		// for a total of 10. merges
		ArrayList<Occurrence> keyList1 = keywordsIndex.get(kw1);
		ArrayList<Occurrence> keyList2 = keywordsIndex.get(kw2);

		// The boolean ( i < 5 && i < keyList1.size())
		// allows us to loop up to 5 times, or until we are at the capacity
		ArrayList<Occurrence> merged = new ArrayList<Occurrence>();
		if (keyList1 != null) {
			for (int i = 0; i < 5 && i < keyList1.size(); i++) {
				merged.add(keyList1.get(i));
			}
		}

		// Merging items into sorted arrayList
		if (keyList2 != null) {
			for (int i = 0; i < 5 && i < keyList2.size(); i++) {
				if (merged.isEmpty()) {
					merged.add(keyList2.get(i));
				} else {
					int x = 0;
					while ( x < merged.size() && keyList2.get(i).frequency < merged.get(x).frequency) {
						x++;
					}
					merged.add(x, keyList2.get((i)));
				}

			}
		}

//====================================================================================		
//									TEST CODE
//====================================================================================
		//TEST CODE DO NOT INCLUDE IN FINAL SUBMISSION
		//TEST WHETHER OR NOT THE FINAL LIST IS IN CORRECT ORDER
		boolean isGood = true;
		for(int i = 1; i < merged.size();i++) {
			if( i > 0 && i < merged.size() - 1) {
				if(merged.get(i-1).frequency < merged.get(i).frequency || merged.get(i+1).frequency > merged.get(i).frequency) {
					isGood = false;
				}
			}
		}
		System.out.println("============================================================");
		System.out.println(("Merged result arraylist order is correct: " + isGood).toUpperCase());
		System.out.println("============================================================");
//====================================================================================
//									TEST CODE
//====================================================================================

		// Keep track of which documents are being returned.
		// Also Go through all of merged & return
		HashMap<String, String> documentsReturned = new HashMap<String, String>(10, 1.0f);
		ArrayList<String> documents = new ArrayList<String>();

		
		//Return only the first 5 items
		int j = 0;
		while (documents.size() <= 5 && j < merged.size()) {
			String doc = merged.get(j).document;
			if (documentsReturned.get(doc) == null) {
				documents.add(doc);
				documentsReturned.put(doc, doc);
			}
			j++;
		}

		return documents.size() == 0 ? null : documents;
	}
}
