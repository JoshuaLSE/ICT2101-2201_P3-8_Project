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
                    sysAdminUser.PasswordHash = BCrypt.Net.BCrypt.HashPassword("admin");
                    sysAdminUser.FullName = "System Administrator";
                    sysAdminUser.Role = "Admin";

                    context.Users.Add(sysAdminUser);
                }

                context.SaveChanges(); //Always include this line after every operations (.Add/.Update/.Delete) to udpate the database with the changes
            }
        }
    }
}
