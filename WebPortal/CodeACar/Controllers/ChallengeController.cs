using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CodeACar.Models;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;

namespace CodeACar.Controllers
{
    public class ChallengeController : Controller
    {
        private readonly ApplicationDbContext _context;

        public ChallengeController(ApplicationDbContext context)
        {
            _context = context;
        }

        public IActionResult Index()
        {
            var role = HttpContext.Request.Cookies["role"].ToString();

            if (string.IsNullOrEmpty(role) || role != "Teacher")
            {
                return RedirectToAction("Login", "User");
            }
            else {
                ViewData["role"] = role.ToString();
                var challenges = _context.Challenges.ToList();
                return View("Index", challenges);
            }
        }

        [HttpGet]
        [Route("/Challenge/Create")]
        public ActionResult GotoCreatePage()
        {
            var role = this.HttpContext.Request.Cookies["role"];
            if (string.IsNullOrEmpty(role) || role != "Teacher")
            {
                return RedirectToAction("Login", "User");
            }
            else
            {
                ViewData["role"] = role.ToString();
                return View("CreateChallenge");
            }
        }

        [HttpGet]
        [Route("/Challenge/Edit/{challengeId}")]
        public ActionResult GotoUpdatePage(int challengeId)
        {
            var roleCookie = this.HttpContext.Request.Cookies["role"];
            if (string.IsNullOrEmpty(roleCookie) || roleCookie != "Teacher")
            {
                return RedirectToAction("Login", "User");
            }
            else
            {
                ViewData["role"] = roleCookie; //Store the role of the user inside the view data for authenticating the user
                var challenge = _context.Challenges.FirstOrDefault(c => c.ChallengeId == challengeId);

                return View("EditChallenge", challenge); //Return the Edit Challenge Information View with the given Challenge Id
            }
        }

        [HttpPost]
        [Route("/Challenge/Create")]
        public async Task<IActionResult> CreateChallenge()
        {
            try
            {
                using (StreamReader reader = new StreamReader(Request.Body, Encoding.UTF8))
                {
                    // Read the challenge information from the browser
                    string json = await reader.ReadToEndAsync();

                    var challengeInfo = JsonConvert.DeserializeObject<Challenge>(json);

                    Challenge newChallenge = new Challenge
                    {
                        Name = challengeInfo.Name,
                        ChallengeImage = challengeInfo.ChallengeImage,
                        Solution = challengeInfo.Solution
                    };

                    // Add the Challenge 
                    _context.Challenges.Add(newChallenge);
                    _context.SaveChanges();

                    // Create the response object to be sent back to the browser
                    var responseObj = new { message="Successfully added Challenge" };

                    return Ok(responseObj);
                }
            }
            catch (Exception ex)
            {
                var responseObj = new { message = "Unable to add the challenge. Please contact the system administrator." };

                return BadRequest(responseObj);
            }
        }

        [HttpPost]
        [Route("/Challenge/Update/{challengeId}")]
        public async Task<IActionResult> UpdateChallenge(int challengeId)
        {
            try
            {
                using (StreamReader reader = new StreamReader(Request.Body, Encoding.UTF8))
                {
                    // Read the challenge information from the browser
                    string json = await reader.ReadToEndAsync();

                    var challengeInfo = JsonConvert.DeserializeObject<Challenge>(json);

                    // Get the challenge with the challenge Id from the database
                    var currentChallenge = _context.Challenges.FirstOrDefault(challenge => challenge.ChallengeId == challengeId);

                    if (currentChallenge == null)
                    {
                        var errorResponseObj = new { message = "Unable to update challenge as the challenge does not exist." };

                        return BadRequest(errorResponseObj);
                    }

                    currentChallenge.Name = challengeInfo.Name;
                    currentChallenge.ChallengeImage = challengeInfo.ChallengeImage;
                    currentChallenge.Solution = challengeInfo.Solution;

                    // Update the Challenge 
                    _context.Challenges.Update(currentChallenge);
                    _context.SaveChanges();

                    // Create the response object to be sent back to the browser
                    var responseObj = new { message = "Successfully updated Challenge" };

                    return Ok(responseObj);
                }
            }
            catch (Exception ex)
            {
                var responseObj = new { message = "Unable to update the challenge. Please contact the system administrator." };

                return BadRequest(responseObj);
            }
        }

        [HttpPost]
        [Route("/Challenge/Delete/{challengeId}")]
        public IActionResult DeleteChallenge(int challengeId)
        {
            try
            {
                // Get the challenge with the challenge Id from the database
                var currentChallenge = _context.Challenges.FirstOrDefault(challenge => challenge.ChallengeId == challengeId);

                if (currentChallenge == null)
                {
                    var errorResponseObj = new { message = "Unable to delete challenge as the challenge does not exist." };

                    return BadRequest(errorResponseObj);
                }

                // Update the Challenge 
                _context.Challenges.Remove(currentChallenge);
                _context.SaveChanges();

                // Create the response object to be sent back to the browser
                var responseObj = new { message = "Successfully deleted Challenge" };

                return Ok(responseObj);
            }
            catch (Exception ex)
            {
                var responseObj = new { message = "Unable to delete the challenge. Please contact the system administrator." };

                return BadRequest(responseObj);
            }
        }
    }
}
