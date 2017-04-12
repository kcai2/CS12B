import static java.lang.System.*;

class treemap {

   class tree {
      String key;
      String value;
      tree left;
      tree right;
   }
   tree root = null;

   //java.util.TreeMap <String, String> tree
   //      = new java.util.TreeMap <String, String> ();
   
   public String get (String key) {
      tree curr = root;
      while (true) {
         if (key.compareTo(curr.key) < 0) {
            if (curr.left != null) curr = curr.left;
            else return null;
               if (curr.key == key) return curr.value;
         }
         else if (key.compareTo(curr.key) > 0) {
            if (curr.right != null) curr = curr.right;
            else return null;
               if (curr.key == key) return curr.value;
         } else if (curr.key.equals(key)) {
            return curr.value;
         }
         //return null;
      }
   }
      
   public String put (String key, String value) {
      if (root == null) {
         tree newTree = new tree();
         newTree.key = key;
         newTree.value = value;
         root = newTree;
      }
      tree curr = root;
      while (true) {
         if (key.compareTo(curr.key) < 0) { 
            if (curr.left == null) {
               tree newTree = new tree(); 
               newTree.key = key;
               newTree.value = value;
               curr.left = newTree;
            } else {
               curr = curr.left;
               if (key.equals(curr.key)) return curr.value;
            }
            //return curr.value;
         }
         else if (key.compareTo(curr.key) > 0) {
            if (curr.right == null) {
               tree newTree = new tree();
               newTree.key = key;
               newTree.value = value;
               curr.right = newTree;
            } else {
               curr = curr.right;
               if (key.equals(curr.key)) return curr.value;
            }
            //return curr.value;
         }
         else if (curr.key.equals(key)) {
            curr.value = value;
            return curr.value;
         }
      }
   } //end put function 

   public void debug_tree () {
      debug_tree_recur (root, 0);
   }

   private void debug_tree_recur (tree node, int depth) {
      if (node != null) {
         debug_tree_recur(node.left, depth+1);
         out.printf("%3d \"%s\" \"%s\" %s %s%n", depth, node.key,
               node.value, node.left, node.right);
         debug_tree_recur(node.right,depth+1);
      }
   }

}