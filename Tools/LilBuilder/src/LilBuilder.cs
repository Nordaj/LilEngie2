using System;
using System.IO;
using System.Diagnostics;
using System.Text;

static class LilBuilder
{
	static int Main(string[] args)
	{
		//Test arguments
#if DEBUG
		args = new string[] { "gamemodule", "D:\\Documents\\GitHub\\LilEngie2\\LilEngie2.sln", "D:\\Documents\\GitHub\\LilEngie2\\Sandbox\\bin\\EditorGameDebug\\x64\\", "Sandbox", "EditorGameDebug" };
#endif //DEBUG

		//Check the kind of build
		if (args.Get(0) == "gamemodule")
		{
			//Setup locations
			string msbuild;
			string vswhere = "%ProgramFiles(x86)%\\Microsoft Visual Studio\\Installer\\vswhere.exe";
			vswhere = Environment.ExpandEnvironmentVariables(vswhere);

			//Locate MSBuild.exe
			using (Process vswhereProcess = new Process())
			{
				//Setup the process to be started
				vswhereProcess.StartInfo.FileName = vswhere;
				vswhereProcess.StartInfo.UseShellExecute = false;
				vswhereProcess.StartInfo.RedirectStandardOutput = true;
				vswhereProcess.StartInfo.Arguments = "-latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\\**\\Bin\\MSBuild.exe";

				//Try to start the process
				try
				{ vswhereProcess.Start(); }
				catch
				{
					Console.WriteLine("Could not start vswhere.exe process");
					return 1;
				}

				//Wait for exit and collect msbuild location
				vswhereProcess.WaitForExit();
				msbuild = vswhereProcess.StandardOutput.ReadLine();
			}

			//Determine version name (probably using date+time, or we could increment)
			string versionName = args.Get(3) + "-" + GenUID();

			//Create/overwrite LatestVersions.txt with version info
			using (StreamWriter sw = new StreamWriter(File.Create(args.Get(2) + "LatestVersion.txt")))
			{
				sw.WriteLine(versionName);	//Output name
				sw.WriteLine(args.Get(3));	//Game module name
				sw.WriteLine(args.Get(4));	//Configuration
			}

			//Build game module to output directory with the set name using MSBuild.exe
			using (Process msbuildProcess = new Process())
			{
				//Setup the process to be started
				msbuildProcess.StartInfo.FileName = msbuild;
				msbuildProcess.StartInfo.UseShellExecute = false;
				msbuildProcess.StartInfo.RedirectStandardOutput = true;
				msbuildProcess.StartInfo.Arguments = $"{args.Get(1)} -t:{args.Get(3)} -p:TargetName={versionName} -p:Platform=X64 -p:Configuration={args.Get(4)}";

				//Try to start the process
				try
				{ msbuildProcess.Start(); }
				catch
				{
					Console.WriteLine("Could not start MSBuild.exe process");
					return 1;
				}

				string output = msbuildProcess.StandardOutput.ReadToEnd();
				int exitCode = msbuildProcess.ExitCode;
			}

			//Output file name
			Console.WriteLine(versionName);
		}
		else if (args.Get(0) == "gamebuild")
		{
			//TODO when handling full game release builds
		}

		return 0;
	}

	static T Get<T>(this T[] arr, int index)
	{
		if (arr.Length > index)
			return arr[index];
		return default;
	}
	
	static string GenUID()
	{
		Random rand = new Random();

		//Creates string of 5 digits 0-9
		StringBuilder sb = new StringBuilder();
		for (int i = 0; i < 5; i++)
			sb.Append((char)rand.Next(48, 58));

		return sb.ToString();
	}
}
