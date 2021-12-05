<h1 align = "center"> <img src="https://github.com/devicons/devicon/blob/master/icons/visualstudio/visualstudio-plain.svg" alt="php" width="35" height="35"/> </a>ICT2101/2201 Group P3-8 - CodeACar <a href="https://www.php.net" target="_blank">  <a href="https://www.w3.org/html/" target="_blank"> <img src="https://res.cloudinary.com/dkhzest5h/image/upload/v1638709328/c--4_ktwfjp.svg" alt="html5" width="35" height="35"/> </a> </h1><br>

# Prerequisite
### IDE
* We recommend using Visual Studio Community 2019 or newer to run this project. You may get the latest Visual Studio IDE [here](https://visualstudio.microsoft.com/downloads/)
* As the project is coded using the ASP .NET MVC Framework, you will need to check the `ASP.NET and web development` workload as shown in the image below

<img src="https://res.cloudinary.com/dkhzest5h/image/upload/v1638710323/02_ASP.Net_and_web_development_eooshu.png" alt="option"/>

* You will also need a Python IDE, preferably Pycharm to run our flask API. You may get the latest version of Pycharm Community [here](https://www.jetbrains.com/pycharm/download/#section=windows)

### Additional Dependencies
* The project has been coded using the .NET Core 3.1. As such you will need to install the SDK and its Runtime Installer to smoothly run our project. You may get the latest SDK [here](https://dotnet.microsoft.com/download/dotnet/thank-you/sdk-3.1.415-windows-x64-installer) and the runtime [here](https://dotnet.microsoft.com/download/dotnet/thank-you/runtime-aspnetcore-3.1.21-windows-x64-installer).
* The project will also automatically install the following dependencies needed through Visual Studio's NuGet manager:
  1) BCrypt.Net-Next (for encryption of passwords)
  2) Microsoft.EntityFrameworkCore (for database mapper)
  3) Microsoft.EntityFrameworkCore.Design (used by NET Core)
  4) Microsoft.EntityFrameworkCore.Sqlite (for providing database)
  5) Microsoft.EntityFrameworkCore.Tools (used by NET Core)
  6) Newtonsoft.Json (for JSON creating/parsing of data) 

# Running the Project
1. Open our project solution in Visual Studio
2. Right-click the Project's `Solution 'CodeACar'` & Click `Build`
3. Once the IDE has finished building the project, click `Run` or the play icon to launch the web portal.

<img src="https://res.cloudinary.com/dkhzest5h/image/upload/v1638714739/Screenshot_2021-12-05_222613_w87tx6.png" alt="option"/>

4. To run the Flask API, the `app.py` file in PyCharm
5. Edit the `host='ip address here'`, at line 120 to a locally assigned ip address, or localhost (127.0.0.1) as shown in the image below.
 
 <img src="https://res.cloudinary.com/dkhzest5h/image/upload/v1638721165/flask_djoeqf.png" alt="option"/>

6. Right-click `app.py` and select `Run Flask (app.py)`
7. The Flask website is now accessible via any browser, using the ip address specified in step 4.

# Development Workflow

> + Main Branch
>> + Dev Branch
>>> Individual feature branches
>>>> + Feature/Feature-Name
>>>> + Create Pull Request back to Dev
>>> + Pull Request Approved after Code Review
>> + Pull Request to Main
> + Release Branch

# Diagram Changes from Milestone 1/2
For all the image shown below, the classes/objects highlighted in red were removed from our implementation while classes/objects highlighted in blue have newly added attributes or been modified. You may click on the image for a clearer view!
### Class Diagram
We have removed the Dashboard entity as we find it to be a redundant entity. We have also added a new attribute (solution) in challenge entity.

<img src="https://res.cloudinary.com/dkhzest5h/image/upload/v1638716155/P3-8_Diagrams_-_Revamped_Class_Diagram_zraohd.png" alt="option"/>

### Use Case Diagram
We have removed the need for the user to register the car as we have automatically integrated it. We also moved the "View Car Statistics" in the "View Command History" use case.

<img src="https://res.cloudinary.com/dkhzest5h/image/upload/v1638717602/P3-8_Diagrams_-_Use_Case_Diagram_q3roub.png" alt="option"/>

# User Acceptance Testing Demo (UAT)
-- will be filled up once josh has recorded the vid --

# White Box Testing (WBT)
Our team will be testing the `ChallengeController` class using Visual Studio's in-built testing framework, XUnit.

### How to Build & Run Unit Test
1. Navigate to the `ChallengeControllerTests.cs` file & right click `Run Tests` as shown in the image below.
2. If the run fails, right cick `ChallengeControllerTests.cs` and click on `Build`
3. Once the solution has finished building, repeat step 1

<img src="https://res.cloudinary.com/dkhzest5h/image/upload/v1638719670/Screenshot_2021-12-05_235412_ltb1cz.png" alt="option"/>

### Unit Test Demo
[![UAT Demo](https://img.youtube.com/vi/17SpftDQOKM/maxresdefault.jpg)](https://www.youtube.com/watch?v=17SpftDQOKM)

### Unit Test Statistics
The table below shows the coverage report of the Challenge Controller class.

<img src="https://res.cloudinary.com/dkhzest5h/image/upload/v1638730309/test_table_zfaknc.png" alt="option"/>

### Control Flow Graph
This is the Control Flow Graph for our Challenge Controller Class:

![CFGIMAGE](https://res.cloudinary.com/dkhzest5h/image/upload/v1638720086/CFG_mljkow.png)

# Project Charts
### Burndown Chart
The image below shows our burndown chart.

![M3BURNDOWNCHART](https://res.cloudinary.com/dkhzest5h/image/upload/v1638726787/burndown_frrzid.png)

### Gantt Chart
The image below shows our gantt chart cropped to only Milestone 3. This is so that the contents can be viewed clearly. For the full burndown chart, head over to our google excel link below.

![M3GANTTCHART](https://res.cloudinary.com/dkhzest5h/image/upload/v1638726579/just_M3_yzxhqb.png)

Full Gantt Chart - [Google Excel Chart](https://docs.google.com/spreadsheets/d/1ffrToW92ubDyzK-C5VmEj9U5471KRce2Ttyc-IQZ98U/edit?usp=sharing)

Head on over to our project board to view our completed progress! -> [Project Board](https://github.com/alamsah2/ICT2101-2201_P3-8_Project/projects/1)<br>
Our team members: [@Alam](https://github.com/users/alamsah2/) [@Hui Qi](https://github.com/lohhuiqi) [@Joshua](https://github.com/JoshuaLSE) [@Matthew](https://github.com/Matt1998)

