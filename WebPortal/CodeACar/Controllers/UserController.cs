using Microsoft.AspNetCore.Mvc;
using CodeACar.Models;
using Microsoft.AspNetCore.Http;
using System.Linq;
using System;

namespace CodeACar.Controllers
{
    public class UserController : Controller
    {
        private readonly ApplicationDbContext _context;

        //Constructor of the UserController that initialises the database in the controller
        public UserController(ApplicationDbContext context)
        {
            _context = context;
        }

        public IActionResult Index()
        {
            var userRole = this.HttpContext.Request.Cookies["role"]; // Get the cookie value containing the role of the user from the web browser
            if (string.IsNullOrEmpty(userRole) || (userRole != "Admin" && userRole != "Teacher" && userRole != "Student")) //remove the role if they are not allowed to access the view
            {
                return RedirectToAction("Login", "User"); //Redirect user back to Home Page if the user has not logged in
            }
            else
            {
                ViewData["role"] = userRole; //Store the role of the user inside the view data for authenticating the user
                return View("index"); //Return the view with the cshtml file "Index.cshtml" in the "User" folder under "Views"
            }

        }

        [HttpGet]
        public IActionResult Login()
        {
            Response.Cookies.Delete("role"); //Delete the cookie "role"
            ViewData.Remove("role"); // Clear the user's info stored within the web portal
            return View("login");
        }

        //Route: /User/Login?username=xxx&password=yyy
        [Route("/User/Login")]
        [HttpPost]
        public IActionResult Login(IFormCollection loginInfo)
        {
            var username = loginInfo["username"].ToString();
            var passwordPlaintext = loginInfo["password"].ToString();

            //Grab the user with the username from the database
            var user = _context.Users.Where(user => user.Username == username).FirstOrDefault();

            //If user does not exist return the user back to the login page with the error message
            if (user == null)
            {
                ViewData["message"] = "You have entered an invalid username/password. Please try again.";
                return View("login");
            }

            //Check if the password is correct
            if (BCrypt.Net.BCrypt.Verify(passwordPlaintext, user.PasswordHash))
            {
                CookieOptions option = new CookieOptions();
                option.Expires = DateTime.Now.AddMinutes(180); //Cookie containing the user information will expires in 3hours from the current datetime

                Response.Cookies.Append("role", user.Role, option); //Create the cookie to the Http Response to save it in the user's web browser

                ViewData["role"] = user.Role; //Save the user's role within the web portal

                return RedirectToAction("Index", "Home"); //Redirect the user to the Dashboard (Index method in Home Controller)
            }
            else //If the password is incorrect, redirect the user back to login page
            {
                ViewData["message"] = "You have entered an invalid username/password. Please try again.";
                return View("login");
            }
        }

        [Route("/User/Logout")]
        [HttpGet]
        public IActionResult Logout()
        {
            Response.Cookies.Delete("role"); //Delete the cookie "role"
            ViewData.Remove("role"); // Clear the user's info stored within the web portal
            return RedirectToAction("Login", "User"); //Redirect user back to the login page
        }
    }
}
