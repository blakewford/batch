import java.util.*;

public class Dubitare
{
    private static Map<String, Integer> gDictionary = new TreeMap<String, Integer>();
    public static void main(String[] args) 
    {
        Scanner scan = new Scanner(System.in);
        int lineCount; int a; int b;
        int testCases = Integer.parseInt(scan.next());
        int cases = testCases;
        int test = 1;
        while(cases-- > 0)
        {
            lineCount = Integer.parseInt(scan.next());
            a = Integer.parseInt(scan.next());
            b = Integer.parseInt(scan.next());
            scan.nextLine();
            while(lineCount-- > 0)
            {
                String line = scan.nextLine();
                String[] words = line.split(" ");
                for(int i = 0; i < words.length; i++)
                {
                    if(words[i].isEmpty())
                        continue;

                    if(gDictionary.containsKey(words[i]))
                    {
                       Integer value = gDictionary.get(words[i]);
                       gDictionary.put(words[i], value + 1);
                    }
                    else
                    {
                        gDictionary.put(words[i], 1);
                    }
                }
            }
            int size = gDictionary.size();
            String[] keys = gDictionary.keySet().toArray(new String[size]);
            Integer[] values = gDictionary.values().toArray(new Integer[size]);

            Map<Integer, String> rank = new TreeMap<Integer, String>();
            for(int i = 0; i < gDictionary.size(); i++)
            {
                if(!rank.containsKey(values[i]))
                {
                    rank.put(values[i], keys[i]);
                    gDictionary.remove(keys[i]);
                    keys = gDictionary.keySet().toArray(new String[size]);
                    values = gDictionary.values().toArray(new Integer[size]);
                    i = 0;
                }
            }

            System.out.println("Testcase #"+test+":");
            test++;
            a--;
            int j =0;
            size = rank.size();
            Integer[] k = rank.keySet().toArray(new Integer[size]);
            String[] v = rank.values().toArray(new String[size]);
            for(int i = size-1; i >= 0 && j < b; i--)
            {
                if(j >= a && j < b)
                {
                    System.out.println(v[i]+":"+k[i]);
                }
                keys = gDictionary.keySet().toArray(new String[gDictionary.size()]);
                values = gDictionary.values().toArray(new Integer[gDictionary.size()]);
                for(int p = 0; p < gDictionary.size() && j < b; p++)
                {
                    if(values[p].equals(k[i]))
                    {
                        if(j >= a && j < b)
                        {
                            System.out.println(keys[p]+":"+values[p]); 
                            gDictionary.remove(keys[p]); //No reason to iterate over this entry again
                            keys = gDictionary.keySet().toArray(new String[gDictionary.size()]);
                            values = gDictionary.values().toArray(new Integer[gDictionary.size()]);
                            p = 0;
                            j++;
                        }
                    }
                }
                j++;
            }
            if(test <= testCases) System.out.println("");
            gDictionary.clear();
        }

    }
}
