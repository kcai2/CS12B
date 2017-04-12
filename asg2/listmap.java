// $Id$

import static java.lang.System.out;

import java.util.Iterator;
import java.util.Map.Entry;
import java.util.NoSuchElementException;

class listmap implements Iterable<Entry<String, intqueue>> {

   private class node implements Entry<String, intqueue> {
      String key;
      intqueue queue = new intqueue();
      node link;

      public String getKey() {
         return key;
      }

      public intqueue getValue() {
         return queue;
      }

      public intqueue setValue(intqueue queue) {
         throw new UnsupportedOperationException();
      }
   }

   private node head = null;

   public listmap() {
      // Not needed, since head defaults to null anyway.
   }
   
   

   public void insert(String key, int linenr) {
      //misc.trace("insert", key, linenr);
      // FIXME - create function to create new nodes / get rid of repeating code
      String word = key;
      node prev = null;
      node curr = head;
      int cmp = 1;
      while (curr != null) {
         key = curr.getKey();
         cmp = key.compareTo(word);
         //out.printf("%s", key);
         //out.println();
         if (cmp >= 0) break;
         prev = curr;
         curr = curr.link;
      }
      if (cmp != 0) {
         node tmp = new node();
         tmp.key = word;
         tmp.link = curr;
         tmp.queue.insert(linenr);
         if (prev == null) head = tmp;
         else prev.link = tmp;
      } else {
         //node tmp = new node();
         curr.queue.insert(linenr);
         //tmp.key = word;
         //tmp.link = curr;
         //if (prev == null) head = tmp;
         //else prev.link = tmp;
         //curr = head;
         // intqueue.insert(count);
      }
   } // end insert method
   

   public Iterator<Entry<String, intqueue>> iterator() {
      return new iterator();
   }

   private class iterator implements Iterator<Entry<String, intqueue>> {
      node curr = head;

      public boolean hasNext() {
         return curr != null;
      }

      public Entry<String, intqueue> next() {
         if (curr == null)
            throw new NoSuchElementException();
         node next = curr;
         curr = curr.link;
         return next;
      }

      public void remove() {
         throw new UnsupportedOperationException();
      }

   }

}