public class StringRec {
	// DO NOT DECLARE ANY VARIABLES HERE
	// (you may declare local variables inside methods)

	public static String decompress(String txt) {
		if (txt.length() < 1) {
			return "";
		} else {
			char firstChar = txt.charAt(0);
			if (Character.isDigit(firstChar)) {
				if (firstChar > 48) {
					firstChar--;
					return txt.charAt(1)
							+ decompress(firstChar + txt.substring(1));
				} else {
					return decompress(txt.substring(2));
				}
			} else {
				return txt.substring(0, 1) + decompress(txt.substring(1));
			}
		}
	}
}
