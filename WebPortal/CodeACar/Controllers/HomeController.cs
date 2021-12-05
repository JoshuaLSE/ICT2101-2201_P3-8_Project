using System.Diagnostics;
using CodeACar.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace CodeACar.Controllers
{
    public class HomeController : Controller
    {
        private readonly ILogger<HomeController> _logger;

        public HomeController(ILogger<HomeController> logger)
        {
            _logger = logger;
        }

        public IActionResult Index()
        {
            var userRole = this.HttpContext.Request.Cookies["role"]; // Get the cookie value containing the role of the user from the web browser
            if (string.IsNullOrEmpty(userRole) || (userRole != "Admin" && userRole != "Teacher" && userRole != "Student"))
            {
                return RedirectToAction("Login", "User"); //Redirect user back to Home Page if the user has not logged in
            }
            else
            {
                ViewData["role"] = userRole; //Store the role of the user inside the view data for authenticating the user

                if (userRole == "Admin")
                {
                    return RedirectToAction("ManageTeachers", "Admin"); //Redirect the manage teachers
                }
                else if (userRole == "Teacher")
                {
                    return RedirectToAction("ManageStudents", "Teacher"); //Redirect the manage teachers}
                }
                else if (userRole == "Student")
                {
                    return RedirectToAction("Index", "Student"); //Redirect the user to the Student Home Page
                }
                else
                {
                    return RedirectToAction("Login", "User");
                }
            }
        }

        public IActionResult Privacy()
        {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}