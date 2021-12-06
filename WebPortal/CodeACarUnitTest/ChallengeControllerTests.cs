using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CodeACar.Controllers;
using CodeACar.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Primitives;
using Microsoft.Net.Http.Headers;
using Xunit;

namespace CodeACarUnitTest
{
    public class ChallengeControllerTests
    {
        [Fact]
        public void Index_ReturnViewResult_WhenUserIsTeacher()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext();

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Teacher" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = controller.Index();

                // Assert that the user is returned the Manage Challenge View
                Assert.IsType<ViewResult>(result);
            }
        }

        [Fact]
        public void Index_ReturnRedirectToActionResult_WhenUserIsNotTeacher()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext();

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Admin" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = Assert.IsType<RedirectToActionResult>(controller.Index());

                // Assert that the user is redirected to the User Login Page
                Assert.Equal("Login", result.ActionName);
                Assert.Equal("User", result.ControllerName);
            }
        }

        [Fact]
        public void GotoCreatePage_ReturnViewResult_WhenUserIsTeacher()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext();

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Teacher" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = controller.GotoCreatePage();

                // Assert that the user is returned the Manage Challenge View
                Assert.IsType<ViewResult>(result);
            }
        }

        [Fact]
        public void GotoCreatePage_ReturnRedirectToActionResult_WhenUserIsNotTeacher()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext();

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Admin" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = Assert.IsType<RedirectToActionResult>(controller.GotoCreatePage());

                // Assert that the user is redirected to the User Login Page
                Assert.Equal("Login", result.ActionName);
                Assert.Equal("User", result.ControllerName);
            }
        }

        [Fact]
        public void GotoUpdatePage_ReturnViewResult_WhenUserIsTeacher()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });

                    context.Challenges.Add(new Challenge
                    {
                        ChallengeId = 1,
                        Name = "Challenge 1",
                        ChallengeImage = "https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide4_pls6lg.jpg",
                        Solution = "MOV[4:1:3] MOV[4:1:3]"
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext();

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Teacher" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = controller.GotoUpdatePage(1);

                // Assert that the user is returned the Manage Challenge View
                Assert.IsType<ViewResult>(result);
            }
        }

        [Fact]
        public void GotoUpdatePage_ReturnRedirectToActionResult_WhenUserIsNotTeacher()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext();

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Admin" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = Assert.IsType<RedirectToActionResult>(controller.GotoUpdatePage(1));

                // Assert that the user is redirected to the User Login Page
                Assert.Equal("Login", result.ActionName);
                Assert.Equal("User", result.ControllerName);
            }
        }

        [Fact]
        public async Task CreateChallenge_ReturnOkObjectResult_WhenChallengeInfoCorrect()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Create a mock Json data containing the information of the Challenge input by a Teacher
                var json = "{ 'ChallengeImage':'https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide1_vs0hnu.jpg','Solution':'MOV[2:1:3] MOV[3:1:3] MOV[3:1:3]', Name: 'Challenge Test' }";

                // Assign the mock Json to a mock MemoryStream
                var stream = new MemoryStream(Encoding.UTF8.GetBytes(json));

                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext()
                {
                    Request = { Body = stream, ContentLength = stream.Length }
                };

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Teacher" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = Assert.IsType<OkObjectResult>(await controller.CreateChallenge());

                // Assert if the returned response object message property is an string
                var message = Assert.IsType<string>(result.Value.GetType().GetProperty("message").GetValue(result.Value));

                // Assert if the value of the message property is correct
                Assert.Equal("Successfully added Challenge", message);
            }
        }

        [Fact]
        public async Task CreateChallenge_ReturnBadRequestObjectResult_WhenChallengeInfoIncomplete()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Create a mock Json data containing the information of the Challenge input by a Teacher
                var json = "{ 'ChallengeImage':'https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide1_vs0hnu.jpg','Solution':'MOV[2:1:3] MOV[3:1:3] MOV[3:1:3]', Name: [] }";

                // Assign the mock Json to a mock MemoryStream
                var stream = new MemoryStream(Encoding.UTF8.GetBytes(json));

                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext()
                {
                    Request = { Body = stream, ContentLength = stream.Length }
                };

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Teacher" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = Assert.IsType<BadRequestObjectResult>(await controller.CreateChallenge());

                // Assert if the returned response object message property is an string
                var message = Assert.IsType<string>(result.Value.GetType().GetProperty("message").GetValue(result.Value));

                // Assert if the value of the message property is correct
                Assert.Equal("Unable to add the challenge. Please contact the system administrator.", message);
            }
        }

        [Fact]
        public async Task EditChallenge_ReturnOkObjectResult_WhenChallengeInfoCorrect()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                if (!context.Challenges.Any())
                {
                    context.Challenges.Add(new Challenge
                    {
                        ChallengeId = 1,
                        Name = "Challenge 1",
                        ChallengeImage = "https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide4_pls6lg.jpg",
                        Solution = "MOV[4:1:3] MOV[4:1:3]"
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Create a mock Json data containing the information of the Challenge input by a Teacher
                var json = "{ 'ChallengeImage':'https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide1_vs0hnu.jpg','Solution':'MOV[2:1:3] MOV[3:1:3] MOV[3:1:3]', Name: 'Challenge Test' }";

                // Assign the mock Json to a mock MemoryStream
                var stream = new MemoryStream(Encoding.UTF8.GetBytes(json));

                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext()
                {
                    Request = { Body = stream, ContentLength = stream.Length }
                };

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Teacher" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = Assert.IsType<OkObjectResult>(await controller.UpdateChallenge(1));

                // Assert if the returned response object message property is an string
                var message = Assert.IsType<string>(result.Value.GetType().GetProperty("message").GetValue(result.Value));

                // Assert if the value of the message property is correct
                Assert.Equal("Successfully updated Challenge", message);
            }
        }

        [Fact]
        public async Task EditChallenge_ReturnBadRequestObjectResult_WhenChallengeIdDoesNotExist()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                if (!context.Challenges.Any())
                {
                    context.Challenges.Add(new Challenge
                    {
                        ChallengeId = 1,
                        Name = "Challenge 1",
                        ChallengeImage = "https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide4_pls6lg.jpg",
                        Solution = "MOV[4:1:3] MOV[4:1:3]"
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Create a mock Json data containing the information of the Challenge input by a Teacher
                var json = "{ 'ChallengeImage':'https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide1_vs0hnu.jpg','Solution':'MOV[4:1:3] MOV[2:1:3] MOV[2:1:3] MOV[4:1:3]', Name: 'Challenge Test' }";

                // Assign the mock Json to a mock MemoryStream
                var stream = new MemoryStream(Encoding.UTF8.GetBytes(json));

                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext()
                {
                    Request = { Body = stream, ContentLength = stream.Length }
                };

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Teacher" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = Assert.IsType<BadRequestObjectResult>(await controller.UpdateChallenge(4));

                // Assert if the returned response object message property is an string
                var message = Assert.IsType<string>(result.Value.GetType().GetProperty("message").GetValue(result.Value));

                // Assert if the value of the message property is correct
                Assert.Equal("Unable to update challenge as the challenge does not exist.", message);
            }
        }

        [Fact]
        public async Task EditChallenge_ReturnBadRequestObjectResult_WhenChallengeInfoIncorrect()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                if (!context.Challenges.Any())
                {
                    context.Challenges.Add(new Challenge
                    {
                        ChallengeId = 1,
                        Name = "Challenge 1",
                        ChallengeImage = "https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide4_pls6lg.jpg",
                        Solution = "MOV[4:1:3] MOV[4:1:3]"
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Create a mock Json data containing the information of the Challenge input by a Teacher
                var json = "{ 'ChallengeImage':'https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide1_vs0hnu.jpg','Solution':'MOV[4:1:3] MOV[2:1:3] MOV[2:1:3] MOV[4:1:3]', Name: [] }";

                // Assign the mock Json to a mock MemoryStream
                var stream = new MemoryStream(Encoding.UTF8.GetBytes(json));

                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext()
                {
                    Request = { Body = stream, ContentLength = stream.Length }
                };

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Teacher" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = Assert.IsType<BadRequestObjectResult>(await controller.UpdateChallenge(4));

                // Assert if the returned response object message property is an string
                var message = Assert.IsType<string>(result.Value.GetType().GetProperty("message").GetValue(result.Value));

                // Assert if the value of the message property is correct
                Assert.Equal("Unable to update the challenge. Please contact the system administrator.", message);
            }
        }

        [Fact]
        public void DeleteChallenge_ReturnOkObjectResult_WhenChallengeIdExist()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                if (!context.Challenges.Any())
                {
                    context.Challenges.Add(new Challenge
                    {
                        ChallengeId = 1,
                        Name = "Challenge 1",
                        ChallengeImage = "https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide4_pls6lg.jpg",
                        Solution = "MOV[4:1:3] MOV[4:1:3]"
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext();

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Teacher" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = Assert.IsType<OkObjectResult>(controller.DeleteChallenge(1));

                // Assert if the returned response object message property is an string
                var message = Assert.IsType<string>(result.Value.GetType().GetProperty("message").GetValue(result.Value));

                // Assert if the value of the message property is correct
                Assert.Equal("Successfully deleted Challenge", message);
            }
        }

        [Fact]
        public void DeleteChallenge_ReturnBadRequestObjectResult_WhenChallengeIdDoesNotExist()
        {
            // Configure the in-memory database for the unit test
            var options = new DbContextOptionsBuilder<ApplicationDbContext>()
                .UseInMemoryDatabase(databaseName: "CodeACarMemDatabase")
                .Options;

            // Seed User Accounts into the database
            using (var context = new ApplicationDbContext(options))
            {
                if (!context.Users.Any())
                {
                    context.Users.Add(new User
                    {
                        Id = 1,
                        FullName = "System Administrator",
                        Username = "admin",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Admin",
                    });

                    context.Users.Add(new User
                    {
                        Id = 2,
                        FullName = "Teacher 1",
                        Username = "teacher1",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Teacher",
                    });

                    context.Users.Add(new User
                    {
                        Id = 3,
                        FullName = "Alam Test",
                        Username = "2001776",
                        PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123"),
                        Role = "Student",
                    });
                }
                if (!context.Challenges.Any())
                {
                    context.Challenges.Add(new Challenge
                    {
                        ChallengeId = 1,
                        Name = "Challenge 1",
                        ChallengeImage = "https://res.cloudinary.com/dkhzest5h/image/upload/v1638724343/Slide4_pls6lg.jpg",
                        Solution = "MOV[4:1:3] MOV[4:1:3]"
                    });
                }
                context.SaveChanges();
            }

            // Use clean instance of database with seeded data to conduct the unit test
            using (var context = new ApplicationDbContext(options))
            {
                // Creates the Http Context to mock the Http Request being sent
                var httpContext = new DefaultHttpContext();

                // Creates mock cookies data within the Http Request
                var cookie = new StringValues(new string[] { "role=Teacher" });
                httpContext.Request.Headers.Add(HeaderNames.Cookie, cookie);

                // Assign the mocked Http Context to a mock Controller Context
                var controllerContext = new ControllerContext()
                {
                    HttpContext = httpContext,
                };

                // Assign the mocked Controller Context to a mock Challenge Controller
                var controller = new ChallengeController(context)
                {
                    ControllerContext = controllerContext
                };

                // Call the controller method to start the test
                var result = Assert.IsType<BadRequestObjectResult>(controller.DeleteChallenge(4));

                // Assert if the returned response object message property is an string
                var message = Assert.IsType<string>(result.Value.GetType().GetProperty("message").GetValue(result.Value));

                // Assert if the value of the message property is correct
                Assert.Equal("Unable to delete challenge as the challenge does not exist.", message);
            }
        }
    }
}
