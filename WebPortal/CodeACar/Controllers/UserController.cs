using Microsoft.AspNetCore.Mvc;
using CodeACar.Models;
using Microsoft.AspNetCore.Http;
using System.Linq;
using System;
using System.IO;
using System.Text;
using Newtonsoft.Json;
using System.Threading.Tasks;

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

        [Route("/Admin/ManageTeachers")]
        public IActionResult ManageTeachers()
        {
            var userRole = this.HttpContext.Request.Cookies["role"]; // Get the cookie value containing the role of the user from the web browser
            if (string.IsNullOrEmpty(userRole) || userRole != "Admin") //remove the role if they are not allowed to access the view
            {
                return RedirectToAction("Login", "User"); //Redirect user back to Home Page if the user has not logged in
            }
            else
            {
                ViewData["role"] = userRole; //Store the role of the user inside the view data for authenticating the user
                var teachers = _context.Users.Where(u => u.Role == "Teacher").ToList();
                return View("ManageTeacher", teachers);
            }
        }

        [Route("/Teacher/ManageStudents")]
        public IActionResult ManageStudents()
        {
            var userRole = this.HttpContext.Request.Cookies["role"]; // Get the cookie value containing the role of the user from the web browser
            if (string.IsNullOrEmpty(userRole) || userRole != "Teacher") //remove the role if they are not allowed to access the view
            {
                return RedirectToAction("Login", "User");
            }
            else
            {
                ViewData["role"] = userRole; //Store the role of the user inside the view data for authenticating the user
                var students = _context.Users.Where(u => u.Role == "Student").ToList();
                return View("ManageStudent", students);
            }
        }

        [Route("/User/CreateUser")]
        public IActionResult CreateUser()
        {
            var userRole = this.HttpContext.Request.Cookies["role"]; // Get the cookie value containing the role of the user from the web browser
            if (string.IsNullOrEmpty(userRole) || (userRole != "Admin" && userRole != "Teacher"))
            {
                return RedirectToAction("Login", "User"); //Redirect user back to Home Page if the user has not logged in
            }
            else
            {
                ViewData["role"] = userRole; //Store the role of the user inside the view data for authenticating the user
                return View("CreateUser");
            }
        }

        [Route("/User/UpdateUser/{userId}")]
        public IActionResult UpdateUser(int userId)
        {
            var userRole = this.HttpContext.Request.Cookies["role"]; // Get the cookie value containing the role of the user from the web browser
            if (string.IsNullOrEmpty(userRole) || (userRole != "Admin" && userRole != "Teacher"))
            {
                return RedirectToAction("Login", "User"); //Redirect user back to Home Page if the user has not logged in
            }
            else
            {
                ViewData["role"] = userRole; //Store the role of the user inside the view data for authenticating the user
                var user = _context.Users.FirstOrDefault(u => u.Id == userId);

                if (user == null)
                {
                    if (userRole == "Admin")
                        return RedirectToAction("ManageTeacheres", "User");
                    else
                        return RedirectToAction("ManageStudents", "User");
                }

                return View("EditUser", user);
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
                ViewData["role"] = user.Role; //Save the user's role within the web portal

                CookieOptions option = new CookieOptions();
                option.Expires = DateTime.Now.AddMinutes(180); //Cookie containing the user information will expires in 3hours from the current datetime

                Response.Cookies.Append("role", user.Role, option); //Create the cookie to the Http Response to save it in the user's web browser

                if (user.Role == "Student")
                {
                    Response.Cookies.Append("studentId", user.Id.ToString(), option);
                    return RedirectToAction("Index", "Student"); //Redirect the user to the Student Home Page
                }
                else if (user.Role == "Admin")
                {
                    return RedirectToAction("ManageTeachers", "Admin"); //Redirect the manage teachers
                }
                else if (user.Role == "Teacher")
                {
                    return RedirectToAction("ManageStudents", "Teacher"); //Redirect the manage teachers}
                }
                else
                {
                    return RedirectToAction("Login", "User");
                }
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

        [HttpPost]
        [Route("/User/Create")]
        public async Task<IActionResult> Create()
        {
            using (StreamReader reader = new StreamReader(Request.Body, Encoding.UTF8))
            {
                // Read the challenge information from the browser
                string json = await reader.ReadToEndAsync();

                var userInfo = JsonConvert.DeserializeObject<User>(json);

                if (_context.Users.Any(u => u.Username == userInfo.Username))
                {
                    var errorResponseObj = new { statusCode = 404, message = $"There exist a user with the username '{userInfo.Username}'" };
                    return Ok(errorResponseObj);
                }

                var newUser = new User
                {
                    FullName = userInfo.FullName,
                    Username = userInfo.Username,
                    PasswordHash = BCrypt.Net.BCrypt.HashPassword(userInfo.PasswordHash),
                    Role = userInfo.Role
                };

                _context.Users.Add(newUser);
                _context.SaveChanges();

                var responseObj = new { statusCode = 200, message = "Successfully added new User" };
                return Ok(responseObj);
            }
        }

        [HttpPost]
        [Route("/User/Update/{userId}")]
        public async Task<IActionResult> Update(int userId)
        {
            using (StreamReader reader = new StreamReader(Request.Body, Encoding.UTF8))
            {
                // Read the challenge information from the browser
                string json = await reader.ReadToEndAsync();

                var userInfo = JsonConvert.DeserializeObject<User>(json);
                var currentUser = _context.Users.FirstOrDefault(u => u.Id == userId);

                if (userInfo.Username != currentUser.Username && _context.Users.Any(u => u.Username == userInfo.Username))
                {
                    var errorResponseObj = new { statusCode = 404, message = $"There exist a user with the username '{userInfo.Username}'" };
                    return Ok(errorResponseObj);
                }

                currentUser.FullName = userInfo.FullName;
                currentUser.Username = userInfo.Username;
                currentUser.PasswordHash = BCrypt.Net.BCrypt.HashPassword(userInfo.PasswordHash);

                _context.Users.Update(currentUser);
                _context.SaveChanges();

                var responseObj = new { statusCode = 200, message = "Successfully updated new User" };
                return Ok(responseObj);
            }
        }
    }
}
