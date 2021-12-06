using System;
using System.Linq;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;

namespace CodeACar.Models
{
    public static class DataSeeder
    {
        public static void Initialise(IServiceProvider serverProvider)
        {
            using (var context = new ApplicationDbContext(serverProvider.GetRequiredService<DbContextOptions<ApplicationDbContext>>()))
            {
                if (context.Users.Any())
                {
                    return; //Do not add default data again if there are already data in the Users table
                }
                else
                {
                    //System Administrator Account
                    User sysAdminUser = new User();
                    sysAdminUser.Username = "admin";
                    sysAdminUser.PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123");
                    sysAdminUser.FullName = "System Administrator";
                    sysAdminUser.Role = "Admin";

                    //Facilitator Account
                    User teacher1 = new User();
                    teacher1.Username = "teacher1";
                    teacher1.PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123");
                    teacher1.FullName = "Teacher 1";
                    teacher1.Role = "Teacher";

                    //Student Account
                    User student1 = new User();
                    student1.Username = "2001718";
                    student1.PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123");
                    student1.FullName = "Hui Qi";
                    student1.Role = "Student";

                    //Student Account
                    User student2 = new User();
                    student2.Username = "2001776";
                    student2.PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123");
                    student2.FullName = "Alam";
                    student2.Role = "Student";

                    User student3 = new User();
                    student3.Username = "2002133";
                    student3.PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123");
                    student3.FullName = "Joshua";
                    student3.Role = "Student";

                    User student4 = new User();
                    student4.Username = "2001568";
                    student4.PasswordHash = BCrypt.Net.BCrypt.HashPassword("P@ssw0rd123");
                    student4.FullName = "Matthew";
                    student4.Role = "Student";

                    context.Users.AddRange(sysAdminUser, teacher1, student1, student2, student3, student4);
                }

                context.SaveChanges(); //Always include this line after every operations (.Add/.Update/.Delete) to udpate the database with the changes
            }
        }
    }
}
