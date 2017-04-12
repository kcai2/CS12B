//$Id: jcal.java,v 1.1 2015-01-20 02:57:44-08 - - $
/*
 * Authors: 
 * Cedric Linares (cslinare@ucsc.edu)
 * Kevin Cai (kcai2@ucsc.edu)
 */

//import java's Gregorian Calendar
import java.util.GregorianCalendar;
//import locale utilities
import java.util.Locale;
//so we dont have to keep typing system
import static java.lang.System.*;
//import java's calendar
import static java.util.Calendar.*;
class jcal {
   //the following declaration of ints is taken from the class site
   static final int MONTHS_IN_YEAR = 12;
   static final int WEEKS_IN_MONTH =  6;
   static final int DAYS_IN_WEEK  =  7;
   static final int calendarMonth = GregorianCalendar.MONTH;
   static final int calendarYear = GregorianCalendar.YEAR;
   static final GregorianCalendar bSwap =
      new GregorianCalendar(1752, SEPTEMBER, 14); 
   
   GregorianCalendar calendar = new GregorianCalendar();
   int calmonth, calyear; boolean yearandmonth;
   
   /* 
    * Initializes a Gregorian Calendar
    */
   jcal() {
      calendar.setGregorianChange(bSwap.getTime());
   }
   
   
   /*
   * Takes into consideration the args array length
   * and from that decides whether to print a year 
   * or month.
   */
   public static void main (String[] args) {
      jcal cal = new jcal();
      Locale local = new Locale(args[0]);
      cal.argCheck(args, local);
      if (args.length == 3) {
         int[][] month = cal.makeMonth(cal.calmonth, cal.calyear);
         cal.printMonth(month, local);
      } else if (args.length == 2) {
         int[][][] year = cal.makeYear(cal.calyear); 
         cal.printYear(year, local);
      } else {
         int[][] month = cal.makeMonth(cal.calmonth, cal.calyear);
         cal.printMonth(month, local);
      }
   } //end main method
   
   
   /*
   * Takes parameters month and year, and makes a 2D array. 
   * The dates of the month are placed at the
   * corresponding indexes of the 2D array. A while loop 
   * iterates over each day in the array.
   * Incrementing is done by using the .add() function in
   * GregorianCalendar.
   */
   int[][] makeMonth (int month, int year) {
      int[][] calmonth = new int[WEEKS_IN_MONTH][DAYS_IN_WEEK];
      calendar.set(calendarYear, year);
      calendar.set(calendarMonth, month);
      calendar.set(GregorianCalendar.DAY_OF_MONTH, 1);
      while (month == calendar.get(calendarMonth)) {
         int calDay = calendar.get (GregorianCalendar.DAY_OF_MONTH);  
         int weekDay = calendar.get(GregorianCalendar.DAY_OF_WEEK) - 1;
         int weekNum = calendar.get(GregorianCalendar.WEEK_OF_MONTH) - 1;
         calmonth[weekNum][weekDay] = calDay;
         calendar.add(GregorianCalendar.DAY_OF_MONTH,1);
      }
      calendar.set(calendarYear, year);
      calendar.set(calendarMonth, month);
      return calmonth;
   } //end makeMonth method

   /*
   * Takes the year parameter, and makes a 3D array. 
   * The dates are filled in the corresponding array
   * indexes. The method makeMonth is used to create each
   * of the months.
   */
   int[][][] makeYear (int year) {
      int numOfMonths = MONTHS_IN_YEAR;
      int numOfWeeks = WEEKS_IN_MONTH;
      int numOfDays = DAYS_IN_WEEK;
      int[][][] calyear = new int[numOfMonths][numOfWeeks][numOfDays];
      for (int mCount = 0; mCount < 12; mCount++) {
         calyear[mCount] = makeMonth(mCount, year);
      }
      calendar.set(calendarYear, this.calyear);
      calendar.set(calendarMonth, this.calmonth);
      return calyear;
   } //end makeYear method
   
   /*
    * Takes a 2D array of a month.
    * We used String.format() to center-justify the name of the
    * month with 20 character spaces. We also replaced all the
    * "0"'s with " ". 
    */
   String formatMonth (int[][] month, Locale local) {
      String print = calendar.getDisplayName(calendarMonth, LONG, local);
      print += (yearandmonth)?" " + calyear :"" ;
      int center = 10 + (print.length()/2); //algorithm: for centering
      String padding = "%-" + center + "s";
      print = String.format(padding,print);
      print = String.format("%20s",print) + "%n";
      print += "Su Mo Tu We Th Fr Sa%n";
      for (int[] week: month) {
         for (int day = 0; day < week.length; day++) {
         String format = day == 0? "%2d" : "%3d";
         String date = String.format(format, week[day]);
         print += date;
         }
         print = print.replace(" 0","  ") + "%n";
      }
      return print;
   } //end formatMonth method

   /*
    * Also takes a 2D array of a month.
    * Areas with 20 spaces are removed.
    */  
   void printMonth (int[][] month, Locale locale) {
      String spaces = String.format("%20s"," ");
      String printout = formatMonth(month, locale).replace(spaces,"");
      int k = printout.length() - 4;
      // eliminates spaces at the end of formatMonth
      for (; printout.substring(k-1,k).equals(" ");k--);
      printout = printout.substring(0,k);
      int count = printout.split("%n").length;
      String newlines = "";
      newlines += (count == 8)?"%n":"%n%n";
      printout = printout + newlines;
      out.printf(printout);
   } //end printMonth method
   
   /* 
    * Takes a 3D array of a year.
    * The year is displayed centered on the first line
    * followed by two new lines.
    * The first for loop iterates every 3 months. 
    * The second for loop splits every month by new line characters.
    * The last for loop gives each month the correct spacing and 
    * adds a new line when needed.
    * Outside of the second and last loop, the first loop prints the 
    * rows of 3 months.
    */
   void printYear (int[][][] year, Locale locale) {
      String yearTitle = Integer.toString(calyear);
      //centers the year at the top of the calendar
      int center = 34 + (yearTitle.length()/2);
      String padding = "%-" + center + "s";
      yearTitle = String.format(padding,yearTitle);
      yearTitle = String.format("%67s",yearTitle).substring(0,66);
      out.print(yearTitle + "\n" + "\n");
      for (int i = 0; i < 12; i += 3) {
        calendar.set(calendarMonth, i);
        String[] print = (this.formatMonth(year[i], locale)).split("%n");
        for (int j = i + 1; j < i+3; j++) {
           calendar.set(calendarMonth, j);   
           String month = this.formatMonth(year[j], locale);
           String[] line = month.split("%n");
           for (int l = 0; l < line.length; l++) {
              print[l] += (j % 3 == 0) && (l == 0)?"":"   ";
              print[l] += line[l] +  ((j + 1) % 3 == 0? "\n": "");
           }
        }   
        formatLine(print);
      }
      out.println();
      calendar.set(calendarMonth, calmonth);
   } //end printYear method

   /* 
    * This method goes with the PrintYear method. Each line is 
    * checked for unnecessary spaces and appends a new line after
    * the last character. Lines that are only spaces are deleted.
    */
   void formatLine (String[] print) {
      for (String k: print) {
         int n = 66;
         if (k != print[0]) {
            for (;n > 0 && k.substring(n-1,n).equals(" "); n--);
            k = k.substring(0,n) + "\n";
         }
         String spaces = String.format("%66s"," ");
         out.print(k.replace(spaces,""));
      }
   } //end formatLine method 
   
   /*
    * This function assists the argCheck function.
    */
   void errs (String[] args) {
      if (args.length > 3) {
         misclib.die("usage: cal [-locale] [month] [year]");
      }
      if (calyear < 1 || calyear > 9999) {
         misclib.die("illegal year value: use 1-9999");
      } else if (calmonth < 0 || calmonth > 11) {
         misclib.die("illegal month value; use 1-12");
      }
   } //end errs method

   /* 
    * Takes the input arguments and parses them.
    * The values of calmonth, calyear, local depend on how
    * many arguments there are. Error in input is
    * also checked.
    */
   void argCheck (String[] args, Locale local) {
      calmonth = calendar.get(calendarMonth);
      calyear = calendar.get(calendarYear);
      yearandmonth = true;
      try {
         if (args.length == 3) {
            calmonth = Integer.parseInt (args[1]) - 1;
            calyear = Integer.parseInt (args[2]);
         } else if (args.length == 2) {
            calyear = Integer.parseInt (args[1]);
            yearandmonth = false;
         } else if (args.length == 3) {
            if(!args[0].substring(0,1).equals("-")) {
               misclib.die("usage: cal [-locale] [month] [year]");   
            }
            local = new Locale(args[0].substring(1));
            calmonth = Integer.parseInt(args[1]) - 1;
            calyear = Integer.parseInt(args[2]);
         }
      } catch (NumberFormatException e) {
      misclib.die("Incorrect integer format");
      }
      errs(args);
   } //end argCheck method



} //end class jcal
