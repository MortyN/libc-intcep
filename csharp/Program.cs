namespace csharp;

class Program
{
    static void Main(string[] args)
    {
        string fileName = "filenamejava.txt";
        string content = "Hello";

        try
        {
            // Write to the file
            File.WriteAllText(fileName, content);

            Console.WriteLine("Content successfully written to " + fileName);
        }
        catch (Exception ex)
        {
            Console.WriteLine("An error occurred: " + ex.Message);
        }
    }
}
