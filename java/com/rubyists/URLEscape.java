class URLEscape
{
  native static String escape(String data);
  native static String unescape(String data);

  static
  {
    System.loadLibrary("url_escape");
  }

  public static void main(String[] args)
  {
    System.out.println(URLEscape.unescape("test%26%20+%35def"));
    System.out.println(URLEscape.escape("test&d+ef"));
  }
}
