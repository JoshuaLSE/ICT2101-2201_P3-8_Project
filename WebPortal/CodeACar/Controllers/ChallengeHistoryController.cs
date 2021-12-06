using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CodeACar.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Newtonsoft.Json;

namespace CodeACar.Controllers
{
    public class ChallengeHistoryController : Controller
    {
        private readonly ApplicationDbContext _context;

        public ChallengeHistoryController(ApplicationDbContext context)
        {
            _context = context;
        }

        public IActionResult Index()
        {
            var role = HttpContext.Request.Cookies["role"].ToString();

            if (string.IsNullOrEmpty(role) || role != "Student")
            {
                return RedirectToAction("Login", "User");
            }
            else
            {
                var studentId = int.Parse(HttpContext.Request.Cookies["studentId"].ToString());
                ViewData["role"] = role.ToString();

                var challenges = _context.Challenges.ToList();
                var challengeHistories = _context.ChallengeHistories.Include(c => c.Challenge).Where(c => c.UserId == studentId && challenges.Contains(c.Challenge)).ToList();
                return View("Index", challengeHistories);
            }
        }

        [Route("/ChallengeHistory/View/{challengeId}")]
        public IActionResult ViewChallengeHistory(int challengeId)
        {
            var role = HttpContext.Request.Cookies["role"].ToString();

            if (string.IsNullOrEmpty(role) || role != "Student")
            {
                return RedirectToAction("Login", "User");
            }
            else
            {
                var studentId = int.Parse(HttpContext.Request.Cookies["studentId"].ToString());
                ViewData["role"] = role.ToString();

                var challenges = _context.Challenges.ToList();
                var challengeHistories = _context.ChallengeHistories.Include(c => c.Challenge).Where(c => c.UserId == studentId && c.ChallengeId == challengeId && challenges.Contains(c.Challenge)).ToList();
                return View("Index", challengeHistories);
            }
        }

        [HttpPost]
        [Route("/ChallengeHistory/Create")]
        public async Task<IActionResult> CreateChallengeHistory()
        {
            try
            {
                using (StreamReader reader = new StreamReader(Request.Body, Encoding.UTF8))
                {
                    // Read the challenge information from the browser
                    string json = await reader.ReadToEndAsync();

                    var challengeHistoryInfo = JsonConvert.DeserializeObject<ChallengeHistory>(json);
                    var studentId = int.Parse(HttpContext.Request.Cookies[""].ToString());

                    ChallengeHistory newChallengeHistory = new ChallengeHistory();
                    newChallengeHistory.ChallengeId = challengeHistoryInfo.ChallengeId;
                    newChallengeHistory.IsCompleted = challengeHistoryInfo.IsCompleted;
                    newChallengeHistory.Command = challengeHistoryInfo.Command;

                    // Add the Challenge History
                    _context.ChallengeHistories.Add(challengeHistoryInfo);
                    _context.SaveChanges();

                    // Create the response object to be sent back to the browser
                    var responseObj = new { message = "Successfully added Challenge History" };

                    return Ok(responseObj);
                }
            }
            catch (Exception ex)
            {
                var responseObj = new { message = ex.Message };

                return BadRequest(responseObj);
            }
        }

        [Route("ChallengeHistory/ViewRaw/{historyId}")]
        public IActionResult ViewRaw(int historyId)
        {
            var role = HttpContext.Request.Cookies["role"].ToString();

            if (string.IsNullOrEmpty(role) || role != "Student")
            {
                return RedirectToAction("Login", "User");
            }
            else
            {
                var studentId = int.Parse(HttpContext.Request.Cookies["studentId"].ToString());
                ViewData["role"] = role.ToString();

                var challengeHistories = _context.ChallengeHistories.Include(c => c.Challenge).FirstOrDefault(c => c.ChallengeHistoryId == historyId);
                if (challengeHistories == null)
                    return RedirectToAction("Index", "ChallengeHistory");

                return View("View", challengeHistories);
            }
            
        }
    }
}
